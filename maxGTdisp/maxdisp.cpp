#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include "cv.h"
#include "highgui.h"

int main (int argc, char** argv){
	if(argc < 3) {
		std::cerr << "Usage: ./maxdisp gt_disp is_noc" << std::endl;
		return -1;
	}
	cv::Mat im_gt = cv::imread(argv[1],-1);
	cv::Mat gt;
	int is_noc = std::atoi(argv[2]); 
	im_gt.convertTo(gt, CV_32F, 1/256.);
	char* fullpath = argv[1];
	char* bname = basename(fullpath);
	std::string fname = (reinterpret_cast<char*>(bname));

	float max = im_gt.at<float>(0,0);
	for(int j=0; j< gt.rows; j++){
		for(int i=0;i<gt.cols; i++){
			float d_gt = gt.at<float>(j,i);
			if( d_gt >= max){
				max = d_gt;
			}
		}
	}
	std::cout << "gonna write to file, value: " << max << std::endl;
	std::ofstream myfile;
	if(is_noc)
		myfile.open("/home/bahar/Master/stereo/maxGTdisp/maxdisp_noc.txt", std::ios::out | std::ios::app);
	else
		myfile.open("/home/bahar/Master/stereo/maxGTdisp/maxdisp_occ.txt", std::ios::out | std::ios::app);
	myfile << "image: " << fname << " maxDisp: " << max << std::endl;
	return 0;
}
