#!/bin/sh
START=$(date +%s.%N)

clang++ -H -c skeleton.h -O0
clang++ -H skeleton.cpp -include skeleton.h -o skeleton -O0 -L/usr/X11R6/lib -lm -lpthread -lX11
# clang++ -H edge_explorer2d.cpp -o gradients -fopenmp -O0 -L/usr/X11R6/lib -lm -lpthread -lX11

END=$(date +%s.%N)
DIFF=$(echo "$END - $START" | bc)
echo $DIFF