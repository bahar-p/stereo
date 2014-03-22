CC = g++
CFLAGS = -c
LIB = 
LFLAGS = 
BUILDPATH = ../build
GLFLAGS = -lGL -lglut -lGLU -lm
OPENCVINC = `pkg-config --cflags opencv`
OPENCVLIB = `pkg-config --libs opencv`

all: disparity kitdisp rectify

disparity: disparity.o
	$(CC) -std=c++0x disparity.o -o $(BUILDPATH)/disparity $(OPENCVLIB) $(GLFLAGS)

kitdisp: dispkitti.o
	$(CC) -std=c++0x dispkitti.o -o $(BUILDPATH)/kitdisp $(OPENCVLIB) $(GLFLAGS)

rectify: rectify.o
	$(CC) -std=c++0x rectify.o -o $(BUILDPATH)/rectify $(OPENCVLIB) $(GLFLAGS)

disparity.o: disparity.cpp
	$(CC) -std=c++0x $(CFLAGS) $(GLFLAGS) $(OPENCVINC) disparity.cpp

dispkitti.o: dispkitti.cpp
	$(CC) -std=c++0x $(CFLAGS) $(GLFLAGS) $(OPENCVINC) dispkitti.cpp

rectify.o: rectify.cpp
	$(CC) -std=c++0x $(CFLAGS) $(GLFLAGS) $(OPENCVINC) rectify.cpp

clean:
	rm -rf *.o *~
