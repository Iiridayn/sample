#!/usr/bin/python3
import argparse, sqlite3, atexit, sys, os.path
from eveonline import warp,dist,HIGHSEC,LOWSEC,JITA,JUMPTIME,AU
from time import perf_counter
from collections import deque
from statistics import mean

start_time = perf_counter()
def check_db(path):
    if not os.path.isfile(path):
        raise argparse.ArgumentTypeError("universeDataDx.db file not found")
    return path

# TODO: accept systems by name or number
parser_parent = argparse.ArgumentParser(add_help=False)
parser_parent.add_argument("-w", "--warp", help="the warp speed of your ship in AU/s, default 3", type=float, default=3)
parser_parent.add_argument("-a", "--align", help="the align time of your ship in s, default 10", type=float, default=10)
group = parser_parent.add_mutually_exclusive_group()
group.add_argument("-v", "--verbose", help="increase verbosity", action='count', default=0)
group.add_argument("-q", "--quiet", help="provide minimal output", action='store_true')

db_parent = argparse.ArgumentParser(add_help=False, parents=[parser_parent])
db_parent.add_argument("-d", "--database", help="the filepath to universeDataDx.db, defaults to ./universeDataDx.db", default="universeDataDx.db", type=check_db)
db_parent.add_argument("--avoid", help="systems to avoid", action='append', nargs='+')

# todo - giving this a parent sort of allows optional args before the command (no error, but ignored)
parser = argparse.ArgumentParser(description="Graph search of the EVE cluster for shortest paths")
subparsers = parser.add_subparsers(dest="cmd")
subparsers.required = True

parser_route = subparsers.add_parser("route", help="Calculate route from one system to another - multiple destinations will be treated as waypoints", parents=[db_parent])
parser_route.add_argument("start", help="Starting system")
parser_route.add_argument("end", help="Destination system(s)", nargs='+')

parser_center = subparsers.add_parser("center", help="Find the central system", parents=[db_parent])
parser_center.add_argument("systems", help="a list of systems you'd like to be central to - accepts special values highsec, lowsec, null, highlow, lownull, all, tradehubs, incursions, rat and empire names", nargs='+', default="all")

parser_warp = subparsers.add_parser("warp", help="Calculate time to warp a fixed distance", parents=[parser_parent])
parser_warp.add_argument("distance", help="distance to warp in meters or AU", type=int)
parser_warp.add_argument("unit", help="units - m, km, au", default="m", nargs='?', choices=["m", "km", "au", "AU"])
parser_warp.add_argument("--cold", help="add this if you are not aligned", action='store_true')


args = parser.parse_args()

# time logging
def log(msg, level=1):
    if args.verbose >= level:
        print(str(round(perf_counter()-start_time, 5))+"s:", msg)
atexit.register(log, "Done")

if args.align > 10 and not args.quiet:
    print("You may want to consider the 10s align MWD warp trick for your ship")

if args.cmd == "warp":
    distance = args.distance
    if args.unit == "km":
        distance *= 1000
    elif args.unit.lower() == "au":
        distance *= AU
    warp_time = warp(distance, args.warp)
    if args.cold:
        warp_time += args.align
    print("Warping {0} {1}: {2:g}s".format(args.distance, args.unit, warp_time))
    exit()

# SQLite connection
log("Loading Database")
conn = sqlite3.connect(args.database)
c = conn.cursor()

systems = {}
sql = "SELECT solarSystemID, solarSystemName, security FROM mapSolarSystems"
if args.cmd == "center" and len(args.systems) == 1 and args.systems[0] in ["highsec", "highlow", "lowsec", "lownull", "null"]:
    if args.systems[0] == "highsec":
        sql += " WHERE security >= " + str(HIGHSEC)
    elif args.systems[0] == "highlow":
        sql += " WHERE security >= " + str(LOWSEC)
    elif args.systems[0] == "lowsec":
        sql += " WHERE security >= " + str(LOWSEC) + " AND security < " + str(HIGHSEC)
    elif args.systems[0] == "lownull":
        sql += " WHERE security < " + str(HIGHSEC)
    elif args.systems[0] == "null":
        sql += " WHERE security < " + str(LOWSEC)
system_query = c.execute(sql)
for row in system_query:
    systems[row[0]] = {'name': row[1], 'security': row[2], 'gates': {}}

gates = {} # map gates to systems for edge lookup
gate_query = c.execute("SELECT itemID, solarSystemID, x, y, z FROM mapDenormalize WHERE groupID = 10")
for row in gate_query:
    if row[1] in systems:
        gates[row[0]] = row[1]
        systems[row[1]]['gates'][row[0]] = {'position': row[2:5]}

# edges
jump_query = c.execute("SELECT stargateID, destinationID FROM mapJumps")
for row in jump_query:
    if row[0] in gates and row[1] in gates:
        systems[gates[row[0]]]['gates'][row[0]]['destination'] = row[1], gates[row[1]]

log("Database loaded")
#print(systems[JITA])

if args.cmd == "route":
    pass
elif args.cmd == "center":
    pass

# jump counts
averages = []
total_systems = len(systems)
min_relevance = total_systems/2
if not args.quiet:
    progress = 0
    print("Processing Systems:")
for start in systems:
    if not args.quiet:
        progress += 1
        sys.stdout.write("\r{0}/{1}".format(progress, total_systems))
        sys.stdout.flush()

    # Dijkstra's / Uniform-cost search
    found = {}
    # TODO: start in stations (or planets w/moons) instead of system origin point
    search = deque([(0, start, 0, 0)])
    while search:
        node = search.popleft()
        from_gate, system, cost, jumps = node

        if system in found and found[system][0] < cost:
            if found[system][1] > jumps:
                # hopefully this never gets printed - invalidates BFS, though my cost heuristic is iffy
                log("Found faster longer route from "+systems[start]['name']+" to "+systems[dest_system]['name'])
            continue

        found[system] = (cost, jumps)
        #log("Processing "+systems[system]['name'])

        jumps += 1
        position = systems[system]['gates'][from_gate]['position'] if from_gate else (0,0,0)
        for gateID in systems[system]['gates']:
            if gateID == from_gate:
                continue

            gate = systems[system]['gates'][gateID]
            if 'destination' not in gate:
                continue
            dest_system = gate['destination'][1]

            #print(position, gate['position'], dist(position, gate['position']))
            warp_cost = cost + warp(dist(position, gate['position']), args.warp) + args.align + JUMPTIME

            # don't worry about searching the search list for now - just add the node, we'll skip when pop
            search.append((gate['destination'][0], dest_system, warp_cost, jumps))

        #log("Done w/gates in "+systems[system]['name'])
        #exit()

    #if JITA not in found:
        #continue # can't reach Jita from here
    if len(found) < min_relevance:
        continue # anything meaningful should reach at least half the graph
    #log("Finished with "+systems[start]['name'])
    #log("Average time: "+str(mean(found[key][0] for key in found)))
    #log("Average jumps: "+str(mean(found[key][1] for key in found)))
    averages.append((
        start,
        mean(found[key][0] for key in found),
        mean(found[key][1] for key in found),
        max(found[key][0] for key in found),
        max(found[key][1] for key in found),
    ))

if not args.quiet:
    print()

averages.sort(key=lambda x: x[1])
print("{0:20}\t{1:8}\t{2:8}\t{3:8}\t{4}".format("System Name", "Time(avg)", "Jumps(avg)", "Time(max)", "Jumps(max)"))
for avg in averages[:10]:
    print("{0:20}\t{1:8g}s\t{2:8g}\t{3:8g}\t{4}".format(systems[avg[0]]['name'], avg[1], avg[2], avg[3], avg[4]))
