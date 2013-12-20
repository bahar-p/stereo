#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <math.h>
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
	if(argc < 6 ) {
		cout << "Usage: ./main leftImg rightImg maxDisp focal_l baseline ?LRCheck? ?mask?" << endl;
		return -1;
	}
	Mat mask;
	//Read input images into Matrices
	Mat image_left = imread(argv[1], -1);
   	Mat image_right = imread(argv[2], -1);
	char* fullpath = argv[1];
	char* bname = basename(fullpath);
	char* x = strtok(bname, ".");
	const std::string fname(reinterpret_cast<char*>(bname));
	//cout << "filename: " << x << " sName: " << sName  << endl;
	maxDisp = atoi(argv[3]);
	float focal = atof(argv[4]);
	float baseline = atof(argv[5]);
	if(argc>6) LR = atoi(argv[6]);
	if(argc>7) mask = imread(argv[7],0);
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
		cv::minMaxLoc(dispR, &minv,&maxv);
		//dispR8 = Mat(dispR.size().height, dispR.size().width, CV_8UC1, Scalar::all(0));
		//dispR.convertTo( dispR8, CV_8UC1,255.0/maxDisp);
		//std::cout << "Execution time:  " << double( clock() - tStart) / (double)CLOCKS_PER_SEC<< " seconds." << std::endl;
		/* Refinement */	
		cv::Mat pixflags(dispL.rows, dispL.cols,CV_32S, Scalar::all(0));
		img->findOutliers(dispL, dispR,pixflags,focal, baseline);
		img->regionVoting(dispL, pixflags, 20, 0.4, 5);
		img->findOutliers(dispL, dispR,pixflags,focal, baseline);
		img->interpolate(image_left, dispL, pixflags);
		//Mat br;
		//img->border(dispL, br);
		//img->discAdjust(dispL, fcost, br);
	}
	img->subpxEnhance(fcost,dispL);
	//cerr << "out of subPx" << endl;
	std::cout << "Exec_time: " << double( clock() - tStart) / (double)CLOCKS_PER_SEC<< " seconds." << std::endl;
	double minv1, maxv1;
	cv::minMaxLoc(dispL, &minv1,&maxv1);
	Mat dispL8;
	//cout << "maxv: " << maxv1 << endl;
	//cout << "final disp channels: " << dispL.channels() << " depth: " << dispL.depth() << endl;
	dispL.convertTo( dispL8, CV_8UC1,255.0/maxDisp);
//	imshow( "Img", image_left );                   
//	imshow( "DispL", dispL8 );                   	
//	if(LR) imshow( "DispR", dispR8 ); 
	string fpath1 = "/home/bahar/Master/stereo/Ex1/adcensus/mydisp/" + fname + ".png";
	imwrite(fpath1 , dispL8);
	if(argc>7) {
		Mat d_masked;
		string fpath2 = "/home/bahar/Master/stereo/Ex1/adcensus/dispmasked/" + fname + ".png";
		dispL8.copyTo(d_masked, mask);
		//imshow( "DispMasked", d_masked );                   	
		imwrite(fpath2, d_masked);
	}
	//waitKey(0);
   //	char c = waitKey(10);
//	if (c == ' ')  return 0;
	return 0;
}

