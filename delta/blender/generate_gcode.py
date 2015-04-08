import os
import math
import bpy

ctx = bpy.context
sel = ctx.selected_objects


ml = 10 # min length
up = 72

d1 = 63 #
d2 = 60 # 62
dd = 0.5

G = ""
gt = []
xo = 0.0
yo = 0.0
zo = 0.0
w = 1

min_step = 2;

#cubic bezier value
def cubic(p, t):
	return p[0]*(1.0-t)**3.0 + 3.0*p[1]*t*(1.0-t)**2.0 + 3.0*p[2]*(t**2.0)*(1.0-t) + p[3]*t**3.0

#length of the curve
def seg_length(p):

	length = 0.0
	prec = 100 #precision
	inc = 1.0/prec #increments
	
	#subdivide the curve in 1000 lines and sum its magnitudes
	
	for i in range(0, prec):
		ti = i*inc 
		tf = (i+1)*inc 
		a = cubic(p,ti) 
		b = cubic(p,tf)
		r = (b-a).magnitude 
		length+=r
			
	return length

def generate_g(x=None,y=None, z=None):
    
    global G
    global gt
    global xo, yo, zo
    
    G+="G1"
    
    if x!=None: 
        G+=" X%f" % x
        xo = x
    if y!=None: 
        G+=" Y%f" % y
        yo = y
    if z!=None: 
        G+=" Z%f" % z
        zo = z
    gt.append((xo,yo,zo,))
    G+="\n"



def split_spline(seg, z):
    l = seg_length(seg)
    steps = l/min_step
    steps = math.ceil(steps)
    if steps>0:
        for s in range(0, steps):
            v = cubic(seg,1.0/steps*s)
            generate_g(v[0], v[1], z)
    else:        
        generate_g(seg[0][0], seg[0][1], z)
    

def parse_spline(spline):
    
    f = spline.bezier_points[0]
    
    generate_g(f.co[0], f.co[1],up)
    
    z = d1
    
    generate_g(None, None, z)
    
    for i in range(0, spline.point_count_u-1):
        p = spline.bezier_points[i  ]
        n = spline.bezier_points[i+1]
        seg = (p.co,p.handle_right, n.handle_left,n.co,)
        split_spline(seg, z)

    if spline.use_cyclic_u :
        p = spline.bezier_points[spline.point_count_u-1]
        n = spline.bezier_points[0]
        seg = (p.co,p.handle_right, n.handle_left,n.co,)
        split_spline(seg, z)
        # last point (first one) cyclic
        generate_g(n.co[0], n.co[1], z)
    else:
        # last poin not cyclic
        p = spline.bezier_points[spline.point_count_u-1]
        generate_g(p.co[0], p.co[1], z)

    generate_g(None, None, up)

def MakePolyLine(objname, curvename, cList):    
    curvedata = bpy.data.curves.new(name=curvename, type='CURVE')    
    curvedata.dimensions = '3D'    
    
    objectdata = bpy.data.objects.new(objname, curvedata)    
    objectdata.location = (0,0,0) #object origin    
    bpy.context.scene.objects.link(objectdata)    
    
    polyline = curvedata.splines.new('BEZIER')    
    polyline.bezier_points.add(len(cList)-1)    
    for num in range(len(cList)):    
        pnt = polyline.bezier_points[num]
        pnt.handle_left = pnt.handle_right = pnt.co = cList[num]
        pnt.handle_left_type = pnt.handle_right_type = "FREE"
        
    
    polyline.order_u = len(polyline.points)-1  
    polyline.use_endpoint_u = True  
    
def generate_gcode(s):
    if s.type  == "CURVE":
        for sp in s.data.splines: 
            parse_spline(sp)
    return G
    
G+="G28\n" # GO HOME        
for s in sel:     
    generate_gcode(s)
G+="G28\n" # GO HOME  
    
path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

f = open(path+"/export.g","w")
f.write(G)
f.close()

MakePolyLine("GCodeObject","GCodeCurve",gt)