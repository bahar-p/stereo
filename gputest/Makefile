OBJS = 3dmem.o
CC = /usr/local/cuda-5.0/bin/nvcc
CUDAPATH = /usr/local/cuda-5.0
NVCCFLAGS = -I$(CUDAPATH)/include -arch sm_30
CFLAGS = -c
LIB = 
LFLAGS = -L$(CUDAPATH)/lib
BUILDPATH = ../build
OPENCVINC = `pkg-config --cflags opencv`
OPENCVLIB = `pkg-config --libs opencv`

all: 3dmemGpu
3dmemGpu: $(OBJS)
	$(CC) $(OBJS) -o $(BUILDPATH)/3dmemGpu $(OPENCVLIB) $(LFLAGS)
3dmem.o: 3dmem.cu
	$(CC) $(OPENCVINC) $(NVCCFLAGS) $(CFLAGS) 3dmem.cu

clean:
	rm -rf *.o *~
