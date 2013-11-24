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
	int scale =1 ;
        int delta = 0;
	int ddepth = CV_16S;

	int c;

	/// Load an image
	src = imread( argv[1] );
	if(argc==3){
		gt = imread (argv[2]);
	}
	if( !src.data ) { return -1; }

	GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
	cout << "src depth: " << src.depth() << " src type: " << src.type() << endl;

	//Convert the image to CV_8UC1
	cvtColor( src, src_gray, CV_BGR2GRAY );
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
	addWeighted( abs_grad_x, 1.0, abs_grad_y, 1.0, 0, grad );
	cout << " size: " << grad.type() << " " << grad.depth()<< " " << grad.channels()<< endl;
	Mat fmask(src.size(), CV_8U);
	for(int i=0;i<src.rows;i++){
		for(int j=0; j<src.cols; j++){
		//	if(grad.at<uchar>(i,j) > 50){
				fmask.at<uchar>(i,j) = grad.at<uchar>(i,j);
		//	}
		}
	}
	//ERODE
//	erode(grad, grad, Mat(), Point(-1,-1), 1);
//	dilate(grad, grad, Mat(), Point(-1,-1),2);
	if(argc==3){ 
		bitwise_or(gt,grad, mask);
		imshow( window_name, mask );
		imwrite("/home/bahar/Dataset/masked/mask.png" , mask);
	}
	else {
		imshow( window_name, grad );
		imshow( "mask2", fmask  );
		imwrite("/home/bahar/Dataset/masked/mask.png" , grad);
	}

	waitKey(0);

	return 0;

}
