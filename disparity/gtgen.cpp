#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <iostream>
#include "cv.h"
#include "highgui.h"
#include <GL/glut.h>
#include <GL/gl.h>

int main( int argc, char* argv[]){

	if(argc < 6){
		std::cout << "Usage: ./gtgen width height bg fg distort" << std::endl;
		return -1;
	}
	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	int bg = atoi(argv[3]);
	int fg = atoi(argv[4]);
	int distort = atoi(argv[5]);
	cv::Mat gt_disp(height, width, CV_8UC1, cv::Scalar::all(bg));

	for(int j=103;j<154;j++){
		for(int i=103;i<154;i++){
			gt_disp.at<uchar>(j,i) = fg;
		}
	}
	gt_disp(cv::Rect(103,103,10,10)) = cv::Scalar::all(distort);
	gt_disp(cv::Rect(97,120,10,10)) = cv::Scalar::all(distort);
	cv::imwrite("/home/bahar/Lgt.png",gt_disp);
}
