from xml.dom import minidom
from svg.path import parse_path

import math
import numpy as np
from matplotlib import pyplot as plt
import cv2

G = ""
xo = yo = zo = None
img = cv2.imread('img01_th.png',0)
w,h = img.shape

minl = 10

mr = 3
r = 3.0

up = 65
pmin = 62
pmax = 59

def generate_g(x=None,y=None, z=None):
    
    global G
    global xo, yo, zo
    global w, h

    G+="G1"
    
    if x!=None: 
        G+=" X%f" % ((0.5-x/w)*150)
        xo = x
    if y!=None: 
        G+=" Y%f" % ((0.5-y/w)*150)
        yo = y
    if z!=None: 
        G+=" Z%f" % z
        zo = z
    G+="\n"

svg_file = "img01.svg"
doc = minidom.parse(svg_file)  # parseString also exists
path_strings = [path.getAttribute('d') for path in doc.getElementsByTagName('path')]
objects = [parse_path(p) for p in path_strings ]
doc.unlink()

segments = []
for path in objects:
    segments += [segment for segment in path if segment.length(error=1e-3) > minl]



canvas = np.full((w,h,1), 255, np.uint8)


G+="G28\n"

for s in segments:
    p    = s.start
    x, y = p.real, p.imag
    generate_g(x, y, up)

    l = s.length(error=1e-3)
    steps = math.ceil(l/r)
    dl = 1.0/steps
    d=pmin
    if steps > 1:
        for i in xrange(0, int(steps+1)):
            p    = s.point(i*dl)
            x, y = p.real, p.imag
            m = cv2.mean(img[y-mr:y+mr,x-mr:x+mr])[0]
            b = int((255-m)/255.0*5)
            d+= ((pmin+(pmax-pmin)*(255-m)/255.0)-d)/5.0
            generate_g(x,y,d)

            if m>0:
                cv2.circle(canvas, (int(x),int(y)), b, (int(m)), -1)
    p = s.end
    x, y = p.real, p.imag
    generate_g(x, y, up)

cv2.imshow("img"   ,img   )
cv2.imshow("canvas",canvas)

cv2.waitKey(0)
cv2.destroyAllWindows()

f = open('export.g', 'w')
f.write(G)
f.close()