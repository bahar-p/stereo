#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "cv.h"
#include "highgui.h"


using namespace cv;
using namespace std;
/*
*******
This file is to detect edge in an image which is passed to it as input and 
build an edge mask as output
*******
*/
int main(int argc, char* argv[]){

	Mat src, src_gray, gt, mask;
	Mat grad;
	char* window_name = (char*) "Edge Detector";
	int scale = 1;
        int delta = 0;
	int ddepth = CV_16S;

	int c;

	/// Load an image
	src = imread( argv[1] );
	gt = imread (argv[2]);
	if( !src.data ) { return -1; }

	GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
	cout << "src depth: " << src.depth() << " src type: " << src.type() << endl;

	/// Convert it to gray
	if(src.depth()!=0){
		cout << "converting input image to grayScale image" << endl;
		cvtColor( src, src_gray, CV_BGR2GRAY );
	} else {
		src_gray=src;
	}
	/// Create window
	namedWindow( window_name, CV_WINDOW_AUTOSIZE );

       /// Generate grad_x and grad_y
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_x, abs_grad_x );

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y );

	/// Total Gradient (approximate)
	addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
	bitwise_or(gt,grad, mask);

	imshow( window_name, grad );
	imshow( "final mask", mask );


	waitKey(0);

	return 0;

}
