#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include "image.h"

using namespace std;
using namespace cv;

image *img;

int main(int argc, char **argv)
{
	//Read input images into Matrices
	Mat image_left = imread(argv[1], CV_LOAD_IMAGE_COLOR);
   	Mat image_right = imread(argv[2], CV_LOAD_IMAGE_COLOR);
   	Size s = image_left.size();
   	int minDisp=0, maxDisp=20;
    img = new image(image_left,image_right, minDisp, maxDisp);
    //cost=cv::Mat(s.height, s.width, CV_32FC1,cv::Scalar::all(0));
   // img->read_image();
	//cout << numeric_limits<double>::max()<<endl;
	//cout << numeric_limits<double>::digits10<<endl;
	//cout << numeric_limits<double>::max()<<endl;
	img->costAD();
	
	img->c_census(7,9);
	img->initCost(10,30);
	img->line_segment(20,6,34,17);
	img->aggregateCost();
	cv::Mat disp = img->scanline(1.0,3.0,15);
	
	
	/*
	for(int p=0; p<image_left.rows;p++){
		for(int q=0;q<image_left.cols;q++){
			disp.at<float>(p,q) = (float)disparity[p][q];
			//cout << disp.at<float>(p,q) << "  ,  " << (float)disparity[p][q]<< endl;
		}
		
	}*/
	double minv, maxv;
	//cv::Point minL, maxL;
	cv::minMaxLoc(disp, &minv,&maxv);
	disp.convertTo( disp, CV_8UC1, 255/(maxv-minv));
	
    imshow( "Disp", disp );                   		// Show our image inside it.
    /*namedWindow( "Display Rightwindow", CV_WINDOW_AUTOSIZE );
    imshow( "Display Rightwindow", img->get_image(0)); 
    */
    /*
    unsigned a = 12;
    double b = (double)a;
  //  double c = fabs((double) b-a)/2;
    printf ("b: %f\t , a: %u\t\n " , b, a);
    */
    waitKey(0);
    if (waitKey(10) == 27)  
		return 0;
}

