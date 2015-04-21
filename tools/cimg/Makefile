CC=clang++
LF=-L/usr/X11R6/lib -lm -lpthread -lX11

.DEFAULT: gradients

# CImg.h.gch: CImg.h
# 	$(CC) -H -c CImg.h

gradients: gradients.cpp 	
	$(CC) -include CImg.h $^ -o $@ $(LF) && ./gradients