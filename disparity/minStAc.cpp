#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "cv.h"
#include "iomanip"
#include "highgui.h"
#include <fstream>


using namespace cv;
using namespace std;
/*
*******
This file is to detect edge in an image which is passed to it as input and 
build an edge mask as output
*******
*/

bool isValid(float d){
	bool valid = (d > 0 ? true : false);
	return valid;
}

int main(int argc, char* argv[]){
	
	if(argc<6){
		cout << "Usage: ./gradient gtImg f b width height" << endl;
		return -1;
	}
	Mat gt;
	Mat gtr = imread(argv[1],-1);
	float f = atof(argv[2]);
	float b = atof(argv[3]);
	float width = atof(argv[4]);
	float height = atof(argv[5]);
	gtr.convertTo(gt, CV_32F,1/(256.)); 			//if it's sgbm generate disparity
	char* fullpath = argv[1];
	char* bname = basename(fullpath);
	char* x = strtok(bname, ".");
	const std::string fname(reinterpret_cast<char*>(x));

	fstream of1;
	float pupil_dist = 64; 				//Avg value of Interpupillary distance (mm)
	float c = 206265; 				//constant (1 radian = 206265 arcsecs)
	float INF = std::numeric_limits<float>::infinity();
	float minStAc = 100000;
	for(int j=0; j<gt.rows; j++){
		for(int i=0; i<gt.cols-1; i++){		
			if(isValid(gt.at<float>(j,i+1)) && isValid(gt.at<float>(j,i))){
				float diff1 = fabs(gt.at<float>(j,i) - gt.at<float>(j,i+1));
				if(diff1>0){
				float gtd1 = gt.at<float>(j,i) ;
				float gtd2 = gt.at<float>(j,i+1); 
				float dp1,dp2;
				if(gtd1 !=0)
					dp1 = f*b/(gt.at<float>(j,i));
				else dp1 = INF;
				if(gtd2 !=0)
					dp2 = f*b/(gt.at<float>(j,i+1));
				else dp2 = INF;
				float dpdiff = fabs ( dp2 - dp1 );
				float stAc = (c*pupil_dist*dpdiff)/(dp1*dp1);
				if(stAc < minStAc)
					minStAc = stAc;
				}
			}
		}
	}

	//cout << "Min_detectable: " << diffMin  << " " << xmin << "," << ymin << " |Max detectable: " << diffMax << " " << xmax << ","<< ymax << endl;
	//cout << <<   << " " << xmin << "," << ymin << endl;
	std::string res1; 
	res1 =  "/home/bahar/Master/stereo/eval/results/discMin.txt";
	of1.open(res1, std::ios::out | std::ios::app);
	of1 << fname << " " << minStAc/60 << " " << f << " " << b << " " << width << " " << height << endl;
	of1.close();
	return 0;
}

