import math

HIGHSEC = 0.45
LOWSEC = 0.05
AU = 149597870700
ACCEL = math.log(AU) # target v = k * AU, log(v/k) is therefore log(AU)
JUMPTIME = 12
ALIGN = -math.log(0.25) # align time constant
JITA = 30000142 # must be able to reach this to be in contiguous highsec
trade_hubs = ["Jita", "Amarr", "Dodixie", "Hek", "Rens"]
hauler_danger = [
    "Niarja", "Sivala", "Jita", "Isanamo", "Stacmon", "Amarr", "Uedama",
    "Aufay", "Balle", "Rens", "Madirmilire", "Litiura", "Kaaputenen",
]
lowsec_pvp = ["Amamake", "Tama", "Rancer", "Egghelende", "Old Man Star"]
high_to_null = ["HED-GP", "EC-P8R", "PF-346", "Y-MPWL", "KBP7-G"]

def dist(a, b):
    return math.sqrt((a[0]-b[0])**2 + (a[1]-b[1])**2 + (a[2]-b[2])**2)

def warp(dist, warp):
    accel = ACCEL
    dewarp = min(2, warp/3)
    if dist < AU:
        accel = math.log(dist)
        decel = accel*warp/dewarp # sigh, close enough
        dist = 0
    else:
        dist -= AU
        decel = math.log(0.016*AU/dewarp)/dewarp # 0.016 seems to be close in practice, the devblog says an "offset"
    return (dist/AU+accel)/warp + decel

def align(mass, agility):
    return ALIGN*mass*agility/1000000
