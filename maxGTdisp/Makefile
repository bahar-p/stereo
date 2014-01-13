CC = g++
CFLAGS = -c
LIB = 
LFLAGS = 
BUILDPATH = ../build
GLFLAGS = -lGL -lglut -lGLU -lm
OPENCVINC = `pkg-config --cflags opencv`
OPENCVLIB = `pkg-config --libs opencv`

all: maxdisp

maxdisp: maxdisp.o
	$(CC) maxdisp.o -o $(BUILDPATH)/maxdisp $(OPENCVLIB) $(GLFLAGS)

maxdisp.o: maxdisp.cpp
	$(CC) $(CFLAGS) $(GLFLAGS) $(OPENCVINC) maxdisp.cpp

clean:
	rm -rf *.o *~

