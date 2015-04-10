import os
import math
import bpy

ctx = bpy.context
sel = ctx.selected_objects


ml = 10 # min length
up = 72

d1 = 63 #
d2 = 62 # 62
dd = 0.5

G = ""
gt = []
xo = 0.0
yo = 0.0
zo = 0.0
w = 1

min_step = 2
merge_dist = 5
parsed_splines = []
current_spline = None

#cubic bezier value
def cubic(p, t):
	return p[0]*(1.0-t)**3.0 + 3.0*p[1]*t*(1.0-t)**2.0 + 3.0*p[2]*(t**2.0)*(1.0-t) + p[3]*t**3.0


#length of the curve
def seg_length(p):

	length = 0.0
	prec = 20 #precision
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



def split_segment(seg):

    global current_spline

    # put firstpoint
    current_spline.append((seg[0][0],seg[0][1],))

    l = seg_length(seg)
    steps = l/min_step
    steps = math.ceil(steps)
    # if too long then  insert additional points
    if steps>0:
        for s in range(0, steps):
            v = cubic(seg,1.0/steps*s)
            current_spline.append((v[0], v[1],))
    

def parse_spline(spline):

    global parsed_splines
    global current_spline 

    current_spline = []
    
    # enum in [‘POLY’, ‘BEZIER’, ‘BSPLINE’, ‘CARDINAL’, ‘NURBS’], default ‘POLY’
    if spline.type == "BEZIER": 
        
        f = spline.bezier_points[0]
                
        for i in range(0, spline.point_count_u-1):
            p = spline.bezier_points[i  ]
            n = spline.bezier_points[i+1]
            seg = (p.co,p.handle_right, n.handle_left,n.co,)
            split_segment(seg)

        if not spline.use_cyclic_u :
            # last poin not cyclic put last point
            p = spline.bezier_points[spline.point_count_u-1]
            current_spline.append((p.co[0], p.co[1],))
        else:
            p = spline.bezier_points[spline.point_count_u-1]
            n = spline.bezier_points[0]
            seg = (p.co,p.handle_right, n.handle_left, n.co,)
            split_segment(seg)

            # last point (first one) cyclic
            current_spline.append((n.co[0], n.co[1],))

    parsed_splines.append(current_spline)      

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
    

def generate_gcode():
    
    global G

    G+="G28\n" # home
    for spline in parsed_splines:
        l = len(spline)
        for i in range(0, l):
            p = spline[i]
            if i==0:
                generate_g(x=p[0], y=p[1],z=up) # first point UP
            
            generate_g(x=p[0], y=p[1], z=d2) 

            if i == l-1:
                generate_g(x=p[0], y=p[1],z=up) # UP
    G+="G28\n"


def optimize_splines():
    return

def dist(p1,p2):
    return math.sqrt((p1[0]-p2[0])*(p1[0]-p2[0])+(p1[1]-p2[1])*(p1[1]-p2[1]))

def merge_near_splines():
    
    global parsed_splines

    for s1 in parsed_splines:
        for s2 in parsed_splines: 
            if s1 != s2:
                if dist(s1[-1],s2[0]) < merge_dist:
                    s1+=s2
                    parsed_splines.remove(s2)
                elif dist(s1[-1],s2[-1]) < merge_dist:
                    s2.reverse()
                    s1+=s2
                    parsed_splines.remove(s2)
                elif dist(s1[0], s2[0]) < merge_dist:
                    s1.reverse()
                    s1+=s2
                    parsed_splines.remove(s2)
                elif dist(s1[0], s2[-1]) < merge_dist:
                    s1.reverse()
                    s2.reverse()
                    s1+=s2
                    parsed_splines.remove(s2)

# http://hackaday.io/project/4955-g-code-optimization
# gredy short path 
# 2opt must be here or ant optimisation

def optimize_order():

    global parsed_splines

    for i in range(0,len(parsed_splines)-1):
        s1 = parsed_splines[i]
        min_d = float('inf')
        closest = None
        for j in range(i+1, len(parsed_splines)):
            s2 = parsed_splines[j]
            m = min(dist(s1[-1],s2[0]),dist(s1[-1],s2[-1]),dist(s1[0],s2[0]),dist(s1[0],s2[-1]))
            if m<min_d: 
                min_d = m
                closest = j
        # make closest spline next
        parsed_splines[i+1], parsed_splines[closest] = parsed_splines[closest], parsed_splines[i+1]         
        
        # check orientation of splines
        s2 = parsed_splines[i+1]
        eb = dist(s1[-1],s2[ 0]) # end   - begin
        ee = dist(s1[-1],s2[-1]) # end   - end
        m = min(eb, ee)

        if m == ee: s2.reverse()
           
    return

def parse_curve(curve):
    for spline in curve.data.splines:
        parse_spline(spline)

    # merge_near_splines()
    # optimize_splines()
    optimize_order()

print("-------")
for s in sel:
    if s.type == 'CURVE':
        parse_curve(s)

generate_gcode()
    
path = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

f = open(path+"/export.g","w")
f.write(G)
f.close()

MakePolyLine("GCodeObject","GCodeCurve",gt)