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

int main(int argc, char **argv)
{
	cudaError_t result;
	//size_t mysize = width * height * drange * sizeof(float);
	//cv::Size size = cv::Size(height, width);
	cv::Mat* h_in = new cv::Mat[drange];
	cv::gpu::GpuMat* h_gpin = new cv::gpu::GpuMat[drange];
	cv::gpu::GpuMat* d_gpin = new cv::gpu::GpuMat[drange];
	cv::gpu::GpuMat* d_out = new cv::gpu::GpuMat[drange];

//	h_in = (struct mtype*) malloc(drange*sizeof(struct mtype)); 
	for(int i =0 ; i<drange ; i++){
		h_in[i] = cv::Mat(height,width, CV_32F, cv::Scalar::all(1.0));
		h_gpin[i] = cv::gpu::GpuMat(height, width, CV_32F);
		d_gpin[i] = cv::gpu::GpuMat(height, width, CV_32F);
		d_out[i] = cv::gpu::GpuMat(height, width, CV_32F);
		//cv::gpu::GpuMat plane;
		//plane.upload(h_in[i]);
		h_gpin[i].upload(h_in[i]);
	}

	result = cudaMalloc((void**)&d_gpin,drange*width*height*sizeof(float));

	if (result != cudaSuccess) {
		fprintf(stderr, "Failed to allocate array of GpuMat on device - %s\n", cudaGetErrorString(result));
		return 1;
	}
	
	result = cudaMemcpy(d_gpin, h_gpin, drange*sizeof(cv::Mat), cudaMemcpyHostToDevice);

	if (result != cudaSuccess) {
		fprintf(stderr, "Failed to copy array of gpumats from host to device- %s\n", cudaGetErrorString(result));
		return 1;
	}


	result = cudaMemcpy(d_out, d_gpin, drange*sizeof(cv::Mat), cudaMemcpyDeviceToHost);
	cv::Mat h_out;
	d_out[0].download(h_out);
	std::cout << "test: " << h_out << std::endl;

/*
	//surface<void, cudaSurfaceType3D> surfRef;
	result = cudaBindSurfaceToArray(surfRef, arrayPointer);
	if (result != cudaSuccess) {
		fprintf(stderr, "cudaBindTextureToArray() - failed to bind texture to array - %s\n", cudaGetErrorString(result));
		return 2;
	}

	const dim3 dimBlock(6,6,6);
	const dim3 dimGrid((width + dimBlock.x-1)/ dimBlock.x, (height + dimBlock.y-1)/dimBlock.y, (depth + dimBlock.z-1 )/dimBlock.z);

	costAD<<<dimGrid,dimBlock>>>(d_left, d_right,surfRef, minDisp, maxDisp, subRW, subRH);

	cudaMemcpy3DParms params = {0};
	memset(&params, 0, sizeof(params));
	params.dstPtr.pitch = sizeof(float) * width;
	params.dstPtr.ptr = h_out;
	params.dstPtr.xsize = width;
	params.dstPtr.ysize = height;
	params.srcPos.x = 0;
	params.srcPos.y = 0;
	params.srcPos.z = 0;
	params.srcArray = arrayPointer;
	params.dstPos.x = 0;
	params.dstPos.y = 0;
	params.dstPos.z = 0;
	params.extent.width = width;
	params.extent.depth = depth;
	params.extent.height = height;
	params.kind = cudaMemcpyDeviceToHost;
	result = cudaMemcpy3D(&params);
	if (result != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy3D - failed to copy from array to host buffer for verification - %s\n", cudaGetErrorString(result));
		return 1;
	}

	cudaThreadSynchronize();
	for (int k = 0; k <depth; k++) {
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				printf(" gpuh_DSIResulft: %f \n", h_out[i + j * width+ k*width*height]);
				h_DSI.at<double>(i,j,k) = (double) h_out[i + j * width + k * width * height];
			}
		}
	}
	cudaFreeArray(arrayPointer);*/
	return 0;
}


