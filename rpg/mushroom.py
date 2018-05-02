import fileinput, random

# Mushroom dataset rules
'''
1. cap-shape: bell=b,conical=c,convex=x,flat=f, knobbed=k,sunken=s 
2. cap-surface: fibrous=f,grooves=g,scaly=y,smooth=s 
3. cap-color: brown=n,buff=b,cinnamon=c,gray=g,green=r, pink=p,purple=u,red=e,white=w,yellow=y 
4. bruises?: bruises=t,no=f 
5. odor: almond=a,anise=l,creosote=c,fishy=y,foul=f, musty=m,none=n,pungent=p,spicy=s 
6. gill-attachment: attached=a,descending=d,free=f,notched=n 
7. gill-spacing: close=c,crowded=w,distant=d 
8. gill-size: broad=b,narrow=n 
9. gill-color: black=k,brown=n,buff=b,chocolate=h,gray=g, green=r,orange=o,pink=p,purple=u,red=e, white=w,yellow=y 
10. stalk-shape: enlarging=e,tapering=t 
11. stalk-root: bulbous=b,club=c,cup=u,equal=e, rhizomorphs=z,rooted=r,missing=? 
12. stalk-surface-above-ring: fibrous=f,scaly=y,silky=k,smooth=s 
13. stalk-surface-below-ring: fibrous=f,scaly=y,silky=k,smooth=s 
14. stalk-color-above-ring: brown=n,buff=b,cinnamon=c,gray=g,orange=o, pink=p,red=e,white=w,yellow=y 
15. stalk-color-below-ring: brown=n,buff=b,cinnamon=c,gray=g,orange=o, pink=p,red=e,white=w,yellow=y 
16. veil-type: partial=p,universal=u 
17. veil-color: brown=n,orange=o,white=w,yellow=y 
18. ring-number: none=n,one=o,two=t 
19. ring-type: cobwebby=c,evanescent=e,flaring=f,large=l, none=n,pendant=p,sheathing=s,zone=z 
20. spore-print-color: black=k,brown=n,buff=b,chocolate=h,green=r, orange=o,purple=u,white=w,yellow=y 
21. population: abundant=a,clustered=c,numerous=n, scattered=s,several=v,solitary=y 
22. habitat: grasses=g,leaves=l,meadows=m,paths=p, urban=u,waste=w,woods=d
'''
# note grooves is only cap-surface, which lacks silky
textures = {
    'f': "fibrous", 'g': "grooves", 'y': "scaly",
    'k': "silky", 's': "smooth",
}
# note not all colors exist in all fields, though there are no conflicts
colors = {
    'k': "black",
    'n': "brown",
    'b': "buff",
    'c': "cinnamon",
    'h': "chocolate",
    'g': "gray",
    'r': "green",
    'o': "orange",
    'p': "pink",
    'u': "purple",
    'e': "red",
    'w': "white",
    'y': "yellow",
}
values = [
    # cap-shape
    {
        'b': "bell", 'c': "conical", 'x': "convex",
        'f': "flat", 'k': "knobbed", 's': "sunken",
    },
    # cap-surface
    textures,
    # cap-color
    colors,
    # bruises?
    {
        't': "bruises", 'f': "does not bruise",
    },
    # odor
    {
        'a': "almond", 'l': "anise", 'c': "creosote",
        'y': "fishy", 'f': "foul", 'm': "musty",
        'n': "none", 'p': "pungent", 's': "spicy",
    },
    # gill-attachment
    {
        'a': "attached", 'd': "descending",
        'f': "free", 'n': "notched",
    },
    # gill-spacing
    {
        'c': "close", 'w': "crowded", 'd': "distant",
    },
    # gill-size
    {
        'b': "broad", 'n': "narrow",
    },
    # gill-color
    colors,
    # stalk-shape
    {
        'e': "enlarging", 't': "tapering",
    },
    # stalk-root
    {
        'b': "bulbous", 'c': "club", 'u': "cup",
        'e': "equal", 'z': "rhizomorphs", 'r': "rooted",
        'nan': "unknown",
    },
    # stalk-surface-above-ring
    textures,
    # stalk-surface-below-ring
    textures,
    # stalk-color-above-ring
    colors,
    # stalk-color-below-ring
    colors,
    # veil-type
    {
        'p': "partial", 'u': "universal",
    },
    # veil-color
    colors,
    # ring-number
    {
        'n': "no", 'o': "one", 't': "two",
    },
    # ring-type
    {
        'c': "cobwebby", 'e': "evanescent", 'f': "flaring",
        'l': "large", 'n': "no", 'p': "pendant",
        's': "sheathing", 'z': "zone",
    },
    # spore-print-color
    colors,
    # population
    {
        'a': "abundant", 'c': "clustered", 'n': "numerous",
        's': "scattered", 'v': "several", 'y': "solitary",
    },
    # habitat
    {
        'g': "grasses", 'l': "leaves", 'm': "meadows",
        'p': "paths", 'u': "urban", 'w': "waste", 'd': "woods"
    },
]
description = [
    "has a {} cap shape",
    "has a {} cap surface",
    "has a {} cap color",
    "{} easily",
    "smells {}",
    "has {} gills",
    "has {} gill spacing",
    "has {} gills",
    "has {} gills",
    "has a {} stalk shape",
    "has a {} stalk root",
    "has a {} stalk surface above the ring",
    "has a {} stalk surface below the ring",
    "has a {} stalk color above the ring",
    "has a {} stalk color below the ring",
    "has a {} veil",
    "has a {} veil",
    "has {} rings",
    "has {} rings",
    "has {} spore prints",
    "has {} population clusters",
    "mostly found in {}",
]

mushrooms = []
for line in fileinput.input():
    mushrooms.append(line.replace("'", "").split(','))

# TODO: arg
some = random.sample(mushrooms, 40)

print(sorted(list(colors.values())))
for shroom in some:
    desc = []
    elements = random.sample(range(21), 4)
    for element in elements:
        val = values[element][shroom[element]]
        desc.append(description[element].format(val))
    desc[-1] = "and "+desc[-1]
    desc = "This mushroom "+", ".join(desc)+"."
    print(desc)
