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

bool isNegative(float n){
	return (n<0 ? true : false);
}

int main(int argc, char* argv[]){
	
	if(argc<10){
		cout << "Usage: ./gradient gtImg dispImg dmax pixthresh f b width height adc" << endl;
		return -1;
	}
	Mat gt,disp;
	Mat gtr = imread(argv[1],-1);
	Mat dispr = imread(argv[2],-1);
	float dmax = atof(argv[3]);
	float pixthresh = atof(argv[4]);
	float f = atof(argv[5]);
	float b = atof(argv[6]);
	float width = atof(argv[7]);
	float height = atof(argv[8]);
	int adc = atoi(argv[9]);
	gtr.convertTo(gt, CV_32F,1/(256.)); 			//if it's sgbm generate disparity
	if(adc) dispr.convertTo(disp, CV_32F,dmax/(16.*255)); 		//if it's AdCensus generated disparity
	else dispr.convertTo(disp, CV_32F,dmax/(255.)); 			//if it's sgbm generate disparity
	char* fullpath = argv[1];
	char* bname = basename(fullpath);
	char* x = strtok(bname, ".");
	const std::string fname(reinterpret_cast<char*>(x));

	fstream of1;
	float pupil_dist = 64; 				//Avg value of Interpupillary distance (mm)
	float c = 3437.75; 				//constant (1 radian=3437.75 arcmin)
	//float d1,d2,gtd1,gtd2;
	float goodpixs=0;
	int k = 12;
	//float thresh [2][k];
	float detected[k];
	for(int x=0; x<k; x++){
		//thresh[0][x] = 0;
		//thresh[1][x] = 0;
		detected[x]=0;
	}
	float thresh[12] = {0.001, 0.002, 0.004, 0.008, 0.016, 0.032, 0.064, 0.128, 0.256, 0.512, 1.024, 2.048};
	float INF = std::numeric_limits<float>::infinity();
	//cout << disp(Rect(700,200, 10,10)) << endl;
	//cout << gt(Rect(700,200, 10,10)) << endl;
	for(int j=0; j<gt.rows; j++){
		for(int i=0; i<gt.cols-1; i++){		
			if(isValid(gt.at<float>(j,i+1)) && isValid(gt.at<float>(j,i))){
				if(fabs(gt.at<float>(j,i) - disp.at<float>(j,i)) < pixthresh){
					goodpixs++;
					float diff1 = fabs(gt.at<float>(j,i) - gt.at<float>(j,i+1));
					float diff2 = fabs(disp.at<float>(j,i) - disp.at<float>(j,i+1));
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
					float stAc_detected = (c*pupil_dist*dpdiff)/(dp1*dp1);
					for(int p=0;p<k;p++){
						if(stAc_detected > 0 && stAc_detected < thresh[p]){
							if(diff2>0 && 
							isNegative(gt.at<float>(j,i) - gt.at<float>(j,i+1)) ==
							isNegative(disp.at<float>(j,i) - disp.at<float>(j,i+1))){
								detected[p]++;
							}
						}
					}
				}
			}
		}
	}


	for(int i=0;i<k;i++){
		detected[i] /= std::max((float)goodpixs, 1.0f);
	}

	//cout << "Min_detectable: " << diffMin  << " " << xmin << "," << ymin << " |Max detectable: " << diffMax << " " << xmax << ","<< ymax << endl;
	//cout << <<   << " " << xmin << "," << ymin << endl;
	std::string res1; 
	if(!adc){
		res1 =  "/home/bahar/Master/stereo/eval/results/discsgbm.txt";
	} else {
		res1 =  "/home/bahar/Master/stereo/eval/results/discadc.txt";
	}
	of1.open(res1, std::ios::out | std::ios::app);
	for(int i=0;i<k;i++){
		of1 << fname << " " << i << " " << thresh[i] << " " << detected[i] <<  " " << goodpixs << 
			" " << f << " " << b << " " << width << " " << height << endl;
	}
	of1.close();
	return 0;
}

