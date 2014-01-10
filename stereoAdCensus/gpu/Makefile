OBJS = imageG.o stereoG.o
CC = /usr/local/cuda-5.0/bin/nvcc
CUDAPATH = /usr/local/cuda-5.0
NVCCFLAGS = -I$(CUDAPATH)/include -gencode arch=compute_30,code=sm_30
CFLAGS = -c
LIB = 
LFLAGS = -L$(CUDAPATH)/lib
BUILDPATH = ../../build
OPENCVINC = `pkg-config --cflags opencv`
OPENCVLIB = `pkg-config --libs opencv`

all: adcenGpu
adcenGpu: $(OBJS)
	$(CC) $(OBJS) -o $(BUILDPATH)/adcenGpu $(OPENCVLIB) $(LFLAGS)
imageG.o: imageG.cu imageG.h
	$(CC) $(OPENCVINC) $(NVCCFLAGS) $(CFLAGS) imageG.cu
stereoG.o: stereoG.cu imageG.h
	$(CC) $(OPENCVINC) $(NVCCFLAGS) $(CFLAGS) stereoG.cu

clean:
	rm -rf *.o *~
