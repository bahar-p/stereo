#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include "imageGpu.h"

using namespace std;
using namespace cv;
using namespace cv::gpu;


imageGpu *img;

int main(int argc, char **argv)
{
	float focal = 3740;				//pixel
	float baseline = 160;			//milimeter	
	
	/*cv::Mat p1= (Mat_<float>(3,1) << 1,1,0);
	cv::Mat q1= (Mat_<float>(3,1) << 2,3,1);
	cv::Mat res1;
	gemm(p1,q1, 1, 0, 0, res1, GEMM_2_T);
	cout<< "p1= " << p1 << '\t' << " q1= " << q1 << '\t' << " res1= " << res1 << endl;*/
	
	//Read input images into Matrices
	Mat image_left = imread(argv[1], CV_LOAD_IMAGE_COLOR);
   	Mat image_right = imread(argv[2], CV_LOAD_IMAGE_COLOR);
   	//Mat image_left = Mat(6,6, CV_8U, Scalar::all(0));
   	//Mat image_right = Mat(6,6, CV_8U, Scalar::all(0));
	gpu::GpuMat d_left, d_right;
	Size s = image_left.size();
   	int minDisp=0, maxDisp=17;
    	img = new imageGpu(image_left,image_right, minDisp, maxDisp);
	int width = s.width;
	int height = s.height;
	int depth = maxDisp-minDisp+1;
	
	int h_sz[] = {height, width, depth};
	cv::Mat h_DSI(3, h_sz,CV_64FC1, cv::Scalar::all(0));

	clock_t tStart = clock();
	img->reset();
	
	img->copyHosttoDevice(image_left, d_left);
	img->copyHosttoDevice(image_right , d_right);

	cudaError_t result;
	cudaArray *arrayPointer;
	size_t bytes = width * height * depth * sizeof(float);
	float *h_out;
	float *d_out;

	h_out = (float *)malloc(bytes);
	surface<void,cudaSurfaceType3D> surfRef;
	// initial value
	for (int k = 0; k <depth; k++) {
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				h_out[i + j * width + k * width * height] = -100;
			}
		}
	}

	cudaChannelFormatDesc channelDesc = cudaCreateChannelDesc(32, 0, 0, 0, cudaChannelFormatKindFloat);
	cudaExtent extent;
	extent.width = width;
	extent.depth = depth;
	extent.height = height;
	result = cudaMalloc3DArray(&arrayPointer, &channelDesc, extent, cudaArraySurfaceLoadStore);
	if (result != cudaSuccess) {
		fprintf(stderr, "Texture3D - failed to malloc 3D array - %s\n", cudaGetErrorString(result));
		return 1;
	}

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


/*
	img->c_census(7,9);
	img->initCost(10,30);
	img->line_segment(20.,6.,34.,17.);
	img->aggregateCost();
	
	Mat dispL=cv::Mat(s.height, s.width, CV_32FC1,cv::Scalar::all(0));
	Mat costL=cv::Mat(s.height, s.width, CV_32FC1,cv::Scalar::all(0));
	Mat fcost = img->scanline(1.0,3.0,15, dispL, costL);
	std::cout << "Execution time:  " << double( clock() - tStart) / (double)CLOCKS_PER_SEC<< " seconds." << std::endl;
	
	double minv1, maxv1;                    
	cv::minMaxLoc(dispL, &minv1,&maxv1);
	//Mat dispL8 = Mat(dispL.size().height, dispL.size().width, CV_8UC1, Scalar::all(0));
	Mat dispL8;
	dispL.convertTo( dispL8, CV_8UC1,255.0/maxDisp);
 
	
    imshow( "DispL", dispL8 );                   	
   	
    waitKey(0);
   	char c = waitKey(10);
    if (c == ' ')  */

	cudaFreeArray(arrayPointer);
	return 0;
}


