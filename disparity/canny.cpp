#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;

Mat src, org_src, src_gray;
Mat dst, detected_edges, compmsk, compdst;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio;
int kernel_size = 3;
string window_name = "";
int itr;
/**
   * @function CannyThreshold
    * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
     */
void CannyThreshold(int, void*)
{
	/// Reduce noise with a kernel 3x3
	blur( src_gray, detected_edges, Size(3,3) );
	/// Canny detector
	Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
	    
	/// Using Canny's output as a mask, we display our result
	dst = Scalar::all(0);
	      
	dilate(detected_edges,detected_edges, Mat(), Point(-1,-1), itr);
	bitwise_not(detected_edges, compmsk);
	org_src.copyTo( dst, detected_edges);	//Mask the src based on the detected edges
	org_src.copyTo( compdst, compmsk);	//Mask the src based on the detected edges
//	dst.convertTo(dst, -1,3,0);		
	imshow( window_name, dst );
	imshow( "detected_edges mask", detected_edges );
}

int main(int argc, char* argv[]){
	
	if(argc < 6){
		cout << "Usage: ./canny smoothed_gt original_gt ratio dilate_itr noc ?minThreshold?" << endl;
		return -1;
	}
	/// Load an image
	src = imread( argv[1],-1);
	org_src = imread(argv[2], -1);

	//cout << "src: " << src.type() << " org_src: " << src.type() << endl;
	ratio = atoi(argv[3]); 
	itr = atoi(argv[4]);
	int noc = atoi(argv[5]);
	if (argc==7) lowThreshold = atoi(argv[6]);
	else lowThreshold = 0;
	char* fullpath = argv[2];
	char* bname = basename(fullpath);
	string fname = reinterpret_cast<char*>(bname);
	if( !src.data || !org_src.data ) { 
		cout << "No valid data" << endl;
		return -1; 
	}
	window_name = (char*) fname.c_str(); 
	/// Create a matrix of the same type and size as src (for dst)
	dst.create( org_src.size(), org_src.type() );

	/// Convert the image to grayscale
	if(src.depth()!=CV_8U){
		cout << "converting to gray scale...." <<endl;
		cvtColor( src, src_gray, CV_BGR2GRAY );
	} else {
		src_gray=src;
	}
	/// Create a window
	namedWindow( window_name, CV_WINDOW_AUTOSIZE );

	/// Create a Trackbar for user to enter threshold
	createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

	/// Show the image
	CannyThreshold(lowThreshold, 0);

	/// Wait until user exit program by pressing a key
	waitKey(0);
	string fpath1, fpath2, fpath3,fpath4;
	ofstream myfile;
	if(noc) {
		myfile.open("/home/bahar/Master/stereo/masks/minthr.txt", std::ios::out | std::ios::app);
		fpath1 = "/home/bahar/Master/stereo/masks/disp_noc/" + fname;
		fpath2 = "/home/bahar/Master/stereo/maskgt/disp_noc/" + fname;
		fpath3 = "/home/bahar/Master/stereo/compmsk/disp_noc/" + fname;
		fpath4 = "/home/bahar/Master/stereo/compmskgt/disp_noc/" + fname;
	} else {
		fpath1 = "/home/bahar/Master/stereo/masks/disp_occ/" + fname;
		fpath2 = "/home/bahar/Master/stereo/maskgt/disp_occ/" + fname;
	}
	myfile << "img: " << fname << " min_thr: " << lowThreshold << endl;
	myfile.close();
	imwrite(fpath1, detected_edges);
	imwrite(fpath1, detected_edges);
	imwrite(fpath3 , compmsk);
	imwrite(fpath4 , compdst);
	return 0;
}
