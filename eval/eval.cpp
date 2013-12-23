#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"

bool isValid(float d){
	d>0 ? true : false;
}

int main (int argc, char** argv) {

	if(argc!=4) {
		std::cerr << "Usage: ./eval gt_disp gen_disp maxDisp" << std::endl;
		return -1;
	}
	cv::Mat im_gt = cv::imread(argv[1],-1);
	cv::Mat im_gen = cv::imread(argv[2],-1);
	int dmax = atoi(argv[3]);
	if(!im_gt.data || !im_gen.data){
		std::cerr << "Invalid Image" << std::endl;
		return -1;
	}

	//Get the original disp values
	cv::Mat d_gt, d_gen;
	im_gt.convertTo(d_gt, CV_32F, 1/256.);
	im_gen.convertTo(d_gen, CV_32F, (dmax)/255.);

	int height = d_gt.size().height;
	int width = d_gt.size().width;
	float err_thr = 3;
	int pix_count = 0;
	//std::cout << "d_gen: " << d_gen(cv::Rect(500,200, 10,10)) << std::endl;
	//std::cout << "d_gt: " << d_gt(cv::Rect(500,200, 10,10)) << std::endl;
	if(height != d_gen.size().height || width != d_gen.size().width){
		std::cerr << "disparity image sizes don't match!" << std::endl;
		return -1;
	}
	
	// Compare: get absolute difference
	for(int j=0; j<height;j++){
		for(int i=0; i<width; i++){
			float fd_gt = d_gt.at<uint16_t>(j,i)/256.0;
			float fd_gen = dmax*d_gt.at<uchar>(j,i)/255.0;
			if(isValid(fd_gt)) {
				float err = fabs(fd_gen - fd_gen);
				if(err > err_thr) pix_count++;
			}
		}
	}

}


