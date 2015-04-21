

G="G28\n"
up = 64
miny =-50
maxy = 50
maxz = 62.5
minz = 53
x = y = z = None
for i in range(0, 10):
	x = i*10
	y = miny
	z = maxz
	G+="G1 X%f Y%f Z%f\n" % (x,y,up)
	for j in range(miny, maxy,5):
		y = j
		z+= (minz - z)/10.0  
		G+="G1 X%f Y%f Z%f\n" % (x,y,z)
	G+="G1 X%f Y%f Z%f\n" % (x,y,up)

G+="G28\n"

f = open("export.g","w")
f.write(G)
f.close