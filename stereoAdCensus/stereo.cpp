#include "cv.h"
#include "highgui.h"
#include <stdio.h>
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
	float focal = 3740;				//pixel
	float baseline = 160;			//milimeter	
	
	/*cv::Mat p1= (Mat_<float>(3,1) << 1,1,0);
	cv::Mat q1= (Mat_<float>(3,1) << 2,3,1);
	cv::Mat res1;
	gemm(p1,q1, 1, 0, 0, res1, GEMM_2_T);
	cout<< "p1= " << p1 << '\t' << " q1= " << q1 << '\t' << " res1= " << res1 << endl;*/
	
	//Read input images into Matrices
	Mat image_left = imread(argv[1], CV_LOAD_IMAGE_COLOR);
   	Mat image_right = imread(argv[2], CV_LOAD_IMAGE_COLOR);
   	//Mat image_left = Mat(6,6, CV_8U, Scalar::all(0));
   //Mat image_right = Mat(6,6, CV_8U, Scalar::all(0));
   	Size s = image_left.size();
   	int minDisp=0, maxDisp=16;
    img = new image(image_left,image_right, minDisp, maxDisp);
    //img->read_image();
	//cout << numeric_limits<double>::max()<<endl;
	//cout << numeric_limits<double>::digits10<<endl;
	//cout << numeric_limits<double>::max()<<endl;       	
	/*uint64_t a = 0;
	a <<= 1;
	a+=1;
	a <<= 1;
	a+=1;
	printf("%s \n" , img->itob(a));*/
	
	clock_t tStart = clock();
	bool Rdisp= true;
	img->costAD(Rdisp);
	img -> costCensus(7,9,1);
	img-> costCensus(7,9,0);
	img->c_census(7,9,Rdisp);
	img->initCost(10,30);
	img->line_segment(20.,6.,34.,17.,Rdisp);
	img->aggregateCost();
	
	Mat dispR=cv::Mat(s.height, s.width, CV_32FC1,cv::Scalar::all(0));
	Mat costR=cv::Mat(s.height, s.width, CV_32FC1,cv::Scalar::all(0));
	img->scanline(1.0,3.0,15, dispR, costR,Rdisp);
	std::cout << "Execution time:  " << double( clock() - tStart) / (double)CLOCKS_PER_SEC<< " seconds." << std::endl;
	double minv, maxv;
	cv::minMaxLoc(dispR, &minv,&maxv);
	Mat dispR8 = Mat(dispR.size().height, dispR.size().width, CV_8UC1, Scalar::all(0));
	dispR.convertTo( dispR8, CV_8UC1,255.0/(maxv-minv));
	
	
	 
	tStart = clock();
	img->reset();
	img->costAD();
	img->c_census(7,9);
	img->initCost(10,30);
	img->line_segment(20.,6.,34.,17.);
	img->aggregateCost();
	
	Mat dispL=cv::Mat(s.height, s.width, CV_32FC1,cv::Scalar::all(0));
	Mat costL=cv::Mat(s.height, s.width, CV_32FC1,cv::Scalar::all(0));
	Mat fcost = img->scanline(1.0,3.0,15, dispL, costL);
	std::cout << "Execution time:  " << double( clock() - tStart) / (double)CLOCKS_PER_SEC<< " seconds." << std::endl;
	cv::Mat pixflags(dispL.rows, dispL.cols,CV_32S, Scalar::all(0));
	img->findOutliers(dispL, dispR,pixflags,focal, baseline);
	cv::Mat f;
	//img->fMatrix(pixflags,dispL,f, 16, 1, 0.99);
	img->regionVoting(dispL, pixflags, 20, 0.4, 5);
	img->findOutliers(dispL, dispR,pixflags,focal, baseline);
	img->interpolate(image_left, dispL, pixflags);
	Mat border;
	img->border(dispL, border);
	img->discAdjust(dispL, fcost, border);
	
	double minv1, maxv1;
	cv::minMaxLoc(dispL, &minv1,&maxv1);
	Mat dispL8 = Mat(dispL.size().height, dispL.size().width, CV_8UC1, Scalar::all(0));
	dispL.convertTo( dispL8, CV_8UC1,255.0/(maxv1-minv1));
   /* for (int i = 0; i < image_left.rows; i++)
	{
		for (int j = 0; j < image_left.cols ; j++)
		{
			cout<< "disp: " << disp.at<float>(i,j) << endl;
		}
		
	}*/
	
    imshow( "DispL", dispL8 );                   	
    imshow( "DispR", dispR8 );  
    imshow( "gradient", border );
    imwrite( "/home/bahar/dispLinterpdiscAdjust.png", dispL8 );
    //imwrite( "/home/bahar/dispR.png", dispR8 );           	
    waitKey(0);
   	char c = waitKey(10);
    if (c == ' ')  
		return 0;
}

