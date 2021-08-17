from http.server import BaseHTTPRequestHandler, HTTPServer
import json
import os
from os.path import expanduser
from random import shuffle
from sys import exit, argv

import signal

if len(argv) < 2:
    print(f"USAGE: {argv[0]} <file>")
    exit(1)

pidfile = os.environ.get('XDG_RUNTIME_DIR') + '/pinboard-search.pid'
data = []
datafile = argv[1]

def reload():
    global data
    with open(expanduser(datafile), encoding='utf-8-sig') as f:
        data = json.load(f)

def sighup(num, frame):
    print("SIGHUP: reloading")
    reload()

signal.signal(signal.SIGHUP, sighup);

def check(q, i, tags):
    if q[0] == '-': # simple recurse to negate
        if q[1] == '-': # prevent recursion DoS; can't use --* meaninfully anyway
            return False
        return not check(q[1:], i, tags)

    if q[0] == '+': # match >= count of +
        for tag in tags:
            if tag[:len(q)] == q:
                return True
    elif q == 'p':
        return i["shared"] == "no"
    elif q == 't':
        return i["toread"] == "yes"
    elif q[0:2] == 'u:':
        url = q[2:].lower()
        return -1 != i['href'].find(url)
    elif q[0:2] == 's:':
        search = q[2:].lower()
        return -1 != i['href'].lower().find(search) \
            or -1 != i['description'].lower().find(search) \
            or -1 != i['extended'].lower().find(search)

    return q in tags

def matches(i, and_query, or_groups):
    tags = i["tags"].split(' ')

    for q in and_query:
        if not check(q, i, tags):
            return False

    for or_group in or_groups:
        found = False
        for q in or_group:
            if check(q, i, tags):
                found = True
                break
        if not found:
            return False

    return True

class Server(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response_only(200)
        self.send_header("Content-type", "text/html; charset=utf-8")
        self.end_headers()

        query = self.path.rstrip('/').split('/')[1:]
        qs = ' '.join(query)

        self.wfile.write(b"<!doctype html>\n<html><head><link rel='icon' href='data:,'><title>")
        self.wfile.write(qs.encode())
        self.wfile.write(b"</title><style>")
        self.wfile.write(b"pre {max-width: 60ch; white-space: pre-wrap; font-family: inherit; margin: 0; margin-bottom: 0.5em}")
        self.wfile.write(b"li {margin-bottom: 1em}")
        self.wfile.write(b"p.tags {font-family: monospace; margin: 0}")
        self.wfile.write(b"</style></head><body><h1>")
        self.wfile.write(qs.encode())
        self.wfile.write(b"</h1><ul>")

        limit = 20
        and_query = []
        or_groups = []
        or_open = False
        # TODO: Full logical search syntax? ~~, ~+, ~~~, etc
        # Can do a full union by not requring + on first term
        # ~foo ~+bar ~+baz ~quux ~+zot = (foo bar baz) | (quux zot)
        # Nesting: ~foo ~~bar ~~baz ~quux zot = ((foo (bar | baz)) | quux) zot
        # Error if ~~ not proceeded by ~; many nested or collapse w/o and
        for q in query:
            if q[0:2] == 'l:':
                limit = max(int(q[2:]), 0)
                continue

            # Odd behavior if ~l:x; treated as regular tag
            if q[0] == '~':
                if not or_open:
                    or_groups.append([])
                    or_open = True
                or_groups[-1].append(q[1:])
                continue
            elif or_open:
                or_open = False

            and_query.append(q)

        results = [i for i in data if matches(i, and_query, or_groups)]

        total = len(results)
        # Random sort if limited; default to first 20
        if limit:
            shuffle(results)
            results = results[:limit]

        for item in results:
            link = f"<a href={item['href']}>{item['description']}</a>"
            desc = ""
            if (item['extended']):
                desc = f"<pre>{item['extended']}</pre>"
            tagblock = f"<p class=\"tags\">{item['tags']}</p>"
            self.wfile.write(f"<li>{link}{desc}{tagblock}</li>".encode())

        self.wfile.write(b"</ul>")
        self.wfile.write(f"<p>Mached: {total}</p>".encode())
        self.wfile.write(b"</body></html>")

httpd = HTTPServer(('', 8001), Server)

pidfd = open(pidfile, 'x')
pidfd.write(str(os.getpid()))
pidfd.close()

def sigterm(num, frame):
    print("Graceful shutdown")
    os.remove(pidfile)
    #httpd.server_close()
    #httpd.shutdown()
    exit(0)

signal.signal(signal.SIGTERM, sigterm);
signal.signal(signal.SIGINT, sigterm);

print("PID:", os.getpid())

reload()
httpd.serve_forever()
