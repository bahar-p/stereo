OBJS = image.o stereo.o
CC = /usr/local/cuda/bin/nvcc
CUDAPATH = /usr/local/cuda
NVCCFLAGS = -I$(CUDAPATH)/include -arch sm_20
CFLAGS = -c
LIB = 
LFLAGS = -L$(CUDAPATH)/lib
BUILDPATH = ../build
OPENCVINC = `pkg-config --cflags opencv`
OPENCVLIB = `pkg-config --libs opencv`

all: adcenGpu
adcenGpu: $(OBJS)
	$(CC) $(OBJS) -o $(BUILDPATH)/adcenGpu $(OPENCVLIB) $(LFLAGS)
stereo.o: stereo.cu image.h
	$(CC) $(OPENCVINC) $(NVCCFLAGS) $(CFLAGS) stereo.cu
image.o: image.cu image.h
	$(CC) $(OPENCVINC) $(NVCCFLAGS) $(CFLAGS) image.cu

clean:
	rm -rf *.o *~
