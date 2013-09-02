CC = g++
CFLAGS = -c
LIB = 
LFLAGS = 
BUILDPATH = ../build
GLFLAGS = -lGL -lglut -lGLU -lm
OPENCVINC = `pkg-config --cflags opencv`
OPENCVLIB = `pkg-config --libs opencv`

all: disparity

disparity: disparity.o
	$(CC) -std=c++0x disparity.o -o $(BUILDPATH)/disparity $(OPENCVLIB) $(GLFLAGS)

disparity.o: disparity.cpp
	$(CC) -std=c++0x $(CFLAGS) $(GLFLAGS) $(OPENCVINC) disparity.cpp

clean:
	rm -rf *.o *~
