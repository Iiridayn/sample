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
        shared = ''
        toread = ''
        url = ''
        search = ''
        for q in query:
            if q[0:2] == 'l:':
                limit = max(int(q[2:]), 0)
            elif q[0:2] == 'u:':
                url = q[2:].lower()
            elif q[0:2] == 's:':
                search = q[2:].lower()
            elif q == 'p':
                shared = "no"
            elif q == '-p':
                shared = "yes"
            elif q == 't':
                toread = "yes"
            elif q == '-t':
                toread = "no"
        tag_query = [q for q in query if q not in ['p', '-p', 't', '-t'] and q[0:2] not in ['l:', 's:', 'u:']]

        def match(i):
            tags = i["tags"].split(' ')
            if toread and i["toread"] != toread:
                return False
            if shared and i["shared"] != shared:
                return False

            for t in tag_query:
                if t[0] == '-':
                    if t[1:] in tags:
                        return False
                elif t not in tags:
                    return False

            if url and -1 == i['href'].find(url):
                return False

            if search:
                return -1 != i['href'].lower().find(search) \
                    or -1 != i['description'].lower().find(search) \
                    or -1 != i['extended'].lower().find(search)

            return True

        results = [i for i in data if match(i)]

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

        self.wfile.write(b"</ul></body></html>")

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
