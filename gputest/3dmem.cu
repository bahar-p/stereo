#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include "cuda.h"
#include "cuda_runtime.h"
#include <opencv2/gpu/gpu.hpp>

using namespace std;
using namespace cv;
using namespace cv::gpu;

int drange = 2;
int width = 10;
int height = 10;
cv::Size s = cv::Size(height,width);
int N = width*height;

static __global__ void mykernel(cv::gpu::PtrStepSz<float> src, cv::gpu::PtrStep<float> dst){
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if( x>= src.cols || y >= src.rows) return;

	dst(y,x) = src(y,x) + 2.0;
//	float a = src(y,x) + 2.0;
//	dst.ptr(y)[x] = a;
	return;
}

int d_caller (const cv::gpu::PtrStepSz<float>& src, const cv::gpu::PtrStepSz<float>& dst) {

	unsigned int block_dimX = (unsigned int)std::ceil(src.cols/(float)8);
	unsigned int block_dimY = (unsigned int)std::ceil(src.rows/(float)8);
	dim3 blocks(block_dimX,block_dimY);
	dim3 threads(8,8);
	mykernel<<<blocks, threads>>>(src, dst);
	cudaDeviceSynchronize();
//	}
		//check for error
		cudaError_t err;
		err = cudaGetLastError();
		if(err != cudaSuccess){
			std::cout << "mykernel: CUDA error with kernel: " << cudaGetErrorString(err) << std::endl;
			return 1;
		}
		return 0;
}

int main(int argc, char **argv)
{
	//cudaError_t result;
	cv::Mat* h_in = new cv::Mat[drange];
	cv::Mat* h_out = new cv::Mat[drange];
	cv::gpu::GpuMat* h_gpin = new cv::gpu::GpuMat[drange];
	cv::gpu::GpuMat* d_gpout = new cv::gpu::GpuMat[drange];

	for(int i =0 ; i<drange ; i++){
		h_in[i] = cv::Mat(height,width, CV_32F, cv::Scalar::all(1.0));
		h_out[i] = cv::Mat(height,width, CV_32F);
		h_gpin[i] = cv::gpu::GpuMat(height, width, CV_32F);
		createContinuous(height, width, CV_32F, d_gpout[i]);
		h_gpin[i].upload(h_in[i]);
	}

/*	result = cudaMalloc((void**)&d_gpin,drange*width*height*sizeof(float));
	result = cudaMalloc((void**)&d_gpout,drange*width*height*sizeof(float));
	if (result != cudaSuccess) {
		fprintf(stderr, "Failed to allocate array of GpuMat on device - %s\n", cudaGetErrorString(result));
		return 1;
	}

	result = cudaMemcpy(d_gpin, h_gpin, drange*sizeof(cv::Mat), cudaMemcpyHostToDevice);

	if (result != cudaSuccess) {
		fprintf(stderr, "Failed to copy array of gpumats from host to device- %s\n", cudaGetErrorString(result));
		return 1;
	}
*/

	for(int i=0; i<drange; i++){
		if (d_caller(h_gpin[i] , d_gpout[i])!=0) {
			std::cout << "kernel call unsuccessful! " << std::endl;
			return -1;
		}
		d_gpout[i].download(h_out[i]);
		std::cout << "hout["<< i << "]= " << h_out[i] << std::endl;
	}
	//result = cudaMemcpy(h_gpout, d_gpout, drange*sizeof(cv::Mat), cudaMemcpyDeviceToHost);

	return 0;
}


