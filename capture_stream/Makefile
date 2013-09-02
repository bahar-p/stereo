CC = g++
CFLAGS = -c
LIB = 
LFLAGS = 
BUILDPATH = ../build
OPENCVCONF = `pkg-config --cflags --libs opencv`
#OPENCVFLAGS = `pkg-config --cflags opencv`

all: capture
capture: stcapture.o
	$(CC) stcapture.o -o $(BUILDPATH)/capture $(OPENCVCONF)
stcapture.o: stcapture.cpp
	$(CC) $(CFLAGS) $(OPENCVCONF) stcapture.cpp

clean:
	rm -rf *.o *~
