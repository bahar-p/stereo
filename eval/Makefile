CC = g++
CFLAGS = -c
LIB = 
LFLAGS = 
BUILDPATH = ../build
GLFLAGS = -lGL -lglut -lGLU -lm
OPENCVINC = `pkg-config --cflags opencv`
OPENCVLIB = `pkg-config --libs opencv`

all: eval

eval: eval.o
	$(CC) -std=c++0x eval.o -o $(BUILDPATH)/eval $(OPENCVLIB) $(GLFLAGS)

eval.o: eval.cpp
	$(CC) -std=c++0x $(CFLAGS) $(GLFLAGS) $(OPENCVINC) eval.cpp

clean:
	rm -rf *.o *~
