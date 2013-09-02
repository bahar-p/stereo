OBJS = image.o stereo.o
CC = g++
CFLAGS = -c
LIB = 
LFLAGS = 
BUILDPATH = ../build
OPENCVCONF = `pkg-config --cflags --libs opencv`
#OPENCVFLAGS = `pkg-config --cflags opencv`

all: main
main: $(OBJS)
	$(CC) $(OBJS) -o $(BUILDPATH)/main $(OPENCVCONF)
stereo.o: stereo.cpp image.h
	$(CC) $(CFLAGS) $(OPENCVCONF) stereo.cpp
image.o: image.cpp image.h
	$(CC) $(CFLAGS) $(OPENCVCONF) image.cpp

clean:
	rm -rf *.o *~
