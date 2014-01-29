#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include "image.h"

using namespace std;
using namespace cv;
using namespace cv::gpu;

image *img;

int main(int argc, char **argv)
{	
	double minv, maxv;
   	int minDisp=0, maxDisp;
	int LR = 0;
	float focal, baseline; 
	if(argc < 5 ) {
		cout << "Usage: ./main leftImg rightImg maxDisp is_noc ?mask? ?LRCheck focal_l baseline?" << endl;
		return -1;
	}
	Mat mask;
	//Read input images into Matrices
	Mat image_left = imread(argv[1], -1);
   	Mat image_right = imread(argv[2], -1);
	char* fullpath = argv[1];
	char* bname = basename(fullpath);
	char* x = strtok(bname, ".");
	const std::string fname(reinterpret_cast<char*>(x));
	maxDisp = atoi(argv[3]);
	int noc = atoi(argv[4]);
	if(argc > 5) mask = imread(argv[5],0);
	if(argc > 6) {
		LR = atoi(argv[6]);
		if(argc==7){
			cout << "FocalLength and Baseline are required when passing LRCheck flag. \n" << 
				"Usage: ./main leftImg rightImg maxDisp is_noc ?mask? ?LRCheck focal_l baseline?" << endl;
			return -1;
		}
		focal = atof(argv[7]);
		baseline = atof(argv[8]);
	}
	Size s = image_left.size();
	img = new image(image_left,image_right, minDisp, maxDisp);
	cv::Mat dispR8;
	
	clock_t tStart = clock();	
	cv::Mat* DSI = img->costAD();
	img -> costCensus(7,9,1);
	img-> costCensus(7,9,0);
	img->c_census(7,9);
	img->initCost(DSI, 10,30);
	img->line_segment(20.,6.,34.,17.);
	img->aggregateCost(DSI);
	for(int i=0;i< maxDisp-minDisp+1 ; i++)
		DSI[i].release();
	Mat dispL= cv::Mat(s.height, s.width, CV_32FC1,cv::Scalar::all(0));
	Mat costL= cv::Mat(s.height, s.width, CV_32FC1,cv::Scalar::all(0));
	cv::Mat* fcost = img->scanline(1.0,3.0,15, dispL, costL);
	
	if(LR){
		bool Rdisp= true;
		img->reset();
		DSI = img->costAD(Rdisp);
		img->c_census(7,9,Rdisp);
		img->initCost(DSI, 10,30);
		img->line_segment(20.,6.,34.,17.,Rdisp);
		img->aggregateCost(DSI);
		Mat dispR=cv::Mat(s.height, s.width, CV_32FC1,cv::Scalar::all(0));
		Mat costR=cv::Mat(s.height, s.width, CV_32FC1,cv::Scalar::all(0));
		img->scanline(1.0,3.0,15, dispR, costR,Rdisp);
		//dispR8 = Mat(dispR.size().height, dispR.size().width, CV_8UC1, Scalar::all(0));
		//dispR.convertTo( dispR8, CV_8UC1,255.0/maxDisp);
		/* Refinement */	
		cv::Mat pixflags(dispL.rows, dispL.cols,CV_32S, Scalar::all(0));
		img->findOutliers(dispL, dispR,pixflags,focal, baseline);
		img->regionVoting(dispL, pixflags, 20, 0.4, 5);
		img->findOutliers(dispL, dispR,pixflags,focal, baseline);
		img->interpolate(image_left, dispL, pixflags);
		Mat br;
		img->border(dispL, br);
		img->discAdjust(dispL, fcost, br);
		//imshow( "borders", br );                   	
	}
	img->subpxEnhance(fcost,dispL);
	minMaxLoc(dispL,&minv,&maxv);
	//std::cout << "Exec_time: " << double( clock() - tStart) / (double)CLOCKS_PER_SEC<< " seconds." << std::endl;
	double ex_time =  double( clock() - tStart) / (double)CLOCKS_PER_SEC;
	Mat dispL16, dispL8;
	cout << "minv: " << minv << "  maxv: " << maxv << endl;
	dispL.convertTo( dispL16, CV_16U,16*255.0/maxDisp);
	dispL.convertTo( dispL8, CV_8U,255.0/maxDisp);
	imshow( "DispL", dispL8 );                   	
	//if(LR) imshow( "DispR", dispR8 );
	ofstream myfile, of;
	string fpath1; 
	if(noc){
		myfile.open("/home/bahar/Master/stereo/Ex1/adcensus/maxDisp_noc.txt", std::ios::out | std::ios::app);
		of.open("/home/bahar/Master/stereo/Ex1/adcensus/mydisp/noc/exeTime.txt", std::ios::out | std::ios::app);
		fpath1 = "/home/bahar/Master/stereo/Ex1/adcensus/mydisp/noc/" + fname + ".png";
	}
	else {
		of.open("/home/bahar/Master/stereo/Ex1/adcensus/mydisp/occ/exeTime.txt", std::ios::out | std::ios::app);
		myfile.open("/home/bahar/Master/stereo/Ex1/adcensus/maxDisp_occ.txt", std::ios::out | std::ios::app);
		fpath1 = "/home/bahar/Master/stereo/Ex1/adcensus/mydisp/occ/" + fname+ ".png";
	}
	myfile << "img: " << fname << " Maxdisp_used: " << maxDisp << endl;
	of << "img: " << fname << " Execution_Time: " << ex_time << " sec" << endl;
	myfile.close();
	of.close();
	imwrite(fpath1 , dispL16);
	imwrite("/home/bahar/adcen_disp.png" , dispL8);
	if(argc>5) {
		Mat d_masked;
		string fpath2; 
		if(noc)
			fpath2 = "/home/bahar/Master/stereo/Ex1/adcensus/dispmasked/noc/" + fname + ".png";
		else
			fpath2 = "/home/bahar/Master/stereo/Ex1/adcensus/dispmasked/occ/" + fname + ".png";
		dispL16.copyTo(d_masked, mask);
		//Mat tmp;
		//dispL.copyTo(tmp,mask);
		//cout<< "tmp: " << tmp(Rect(500,150,80,1)) << endl;
		//cout<< "d_masked: " << d_masked(Rect(500,150,80,1)) << endl;
//		imshow( "DispMasked", d_masked );                   	
		imwrite(fpath2, d_masked);
	}

	for(int i=0;i< maxDisp-minDisp+1 ; i++){
		DSI[i].release();
		fcost[i].release();
	}
	delete[] fcost;
	delete[] DSI;
	waitKey(0);
	return 0;
}

