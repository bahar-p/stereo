CC = g++
CFLAGS = -c
LIB = 
LFLAGS = 
GLFLAGS = -lGL -lglut -lGLU -lm
OPENCVCONF = `pkg-config --cflags --libs opencv`
#OPENCVFLAGS = `pkg-config --cflags opencv`

all: disparity

disparity: disparity.o
	$(CC) -std=c++0x disparity.o -o disparity $(OPENCVCONF)$(GLFLAGS)

disparity.o: disparity.cpp
	$(CC) -std=c++0x $(CFLAGS) $(GLFLAGS) $(OPENCVCONF) disparity.cpp

clean:
	rm -rf *.o *~
