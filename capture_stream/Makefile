CC = g++
CFLAGS = -c
LIB = 
LFLAGS = 
BUILDPATH = ../build
OPENCVINC = `pkg-config --cflags opencv`
OPENCVLIB = `pkg-config --libs opencv`

all: capture
capture: stcapture.o
	$(CC) stcapture.o -o $(BUILDPATH)/capture $(OPENCVLIB)
stcapture.o: stcapture.cpp
	$(CC) $(CFLAGS) $(OPENCVINC) stcapture.cpp

clean:
	rm -rf *.o *~
