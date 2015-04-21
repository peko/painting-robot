#
# Cross Platform Makefile
# Compatible with Ubuntu 14.04.1 and Mac OS X
#
#
# if you using Mac OS X:
# You'll need glfw
#    http://www.glfw.org
#

CXX = clang++

OBJS = vectorizer.o imgui_impl_glfw.o
OBJS += dep/imgui/imgui.o

UNAME_S := $(shell uname -s)


ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS = `pkg-config --static --libs glfw3`

	CXXFLAGS = -Idep/imgui/ `pkg-config --cflags glfw3`
	CXXFLAGS += -g # debug
	CXXFLAGS += -Wall
	CXXFLAGS += -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_photo 
	CFLAGS = $(CXXFLAGS)
endif

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all:vectorizer
	@echo Build complete for $(ECHO_MESSAGE)

vectorizer:$(OBJS)
	$(CXX) -o vectorizer $(OBJS) $(CXXFLAGS) $(LIBS)

clean:
	rm $(OBJS)

