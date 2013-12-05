#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "cv.h"
#include "highgui.h"

using namespace cv;
using namespace std;

Mat src, org_src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio;
int kernel_size = 3;
char* window_name = (char*) "Edge Map";
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
	org_src.copyTo( dst, detected_edges);	//Mask the src based on the detected edges
//	dst.convertTo(dst, -1,3,0);		
	imshow( window_name, dst );
	imshow( "detected_edges mask", detected_edges );
}

int main(int argc, char* argv[]){
	
	if(argc != 6){
		cout << "Usage: ./canny smoothed_gt original_gt ratio dilate_itr filename" << endl;
		return -1;
	}
	/// Load an image
	src = imread( argv[1],-1);
	org_src = imread(argv[2], -1);

	cout << "src: " << src.type() << " org_src: " << src.type() << endl;
	ratio = atoi(argv[3]); 
	itr = atoi(argv[4]); 
	stringstream ss;
	ss << argv[5] << ".png";
	string fname = ss.str();
	cout << "fname: " << fname << endl;
	if( !src.data || !org_src.data ) { return -1; }

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
	CannyThreshold(0, 0);

	/// Wait until user exit program by pressing a key
	waitKey(0);
	string fpath1 = "/home/bahar/Dataset/masks/" + fname;
	string fpath2 = "/home/bahar/Dataset/maskedgt/" + fname;
	imwrite(fpath1, detected_edges);
	imwrite(fpath2 , dst);
	return 0;
}
