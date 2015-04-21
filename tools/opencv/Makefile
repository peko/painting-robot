CC=clang++
LFLAGS=-lstdc++ -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_photo

.DEFAULT: distance


distance: distance.cpp
	$(CC) $^ $(LFLAGS) -o $@ && ./$@ "../data/test02.png"

linetracker: linetracker.cpp
	$(CC) $^ $(LFLAGS) -o $@ && ./$@ "../data/test03.jpg"
