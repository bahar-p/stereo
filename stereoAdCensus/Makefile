OBJS = image.o stereo.o
CC = g++
CFLAGS = -c
LIB = 
LFLAGS = 
BUILDPATH = ../build
OPENCVINC = `pkg-config --cflags opencv`
OPENCVLIB = `pkg-config --libs opencv`

all: main
main: $(OBJS)
	$(CC) $(OBJS) -o $(BUILDPATH)/main $(OPENCVLIB)
stereo.o: stereo.cpp image.h
	$(CC) $(CFLAGS) $(OPENCVINC) stereo.cpp
image.o: image.cpp image.h
	$(CC) $(CFLAGS) $(OPENCVINC) image.cpp

clean:
	rm -rf *.o *~
