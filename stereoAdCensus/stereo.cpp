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
	
	
	//Read input images into Matrices
	Mat image_left = imread(argv[1], CV_LOAD_IMAGE_COLOR);
   	Mat image_right = imread(argv[2], CV_LOAD_IMAGE_COLOR);
   	//Mat image_left = Mat(6,6, CV_8U, Scalar::all(0));
   //Mat image_right = Mat(6,6, CV_8U, Scalar::all(0));
   	Size s = image_left.size();
   	int minDisp=0, maxDisp=15;
    img = new image(image_left,image_right, minDisp, maxDisp);
    //cost=cv::Mat(s.height, s.width, CV_32FC1,cv::Scalar::all(0));
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
	img->costAD();
	
	img->c_census(7,9);
	img->initCost(10,30);
	img->line_segment(20.,6.,34.,17.);
	img->aggregateCost();
	
	Mat disp=cv::Mat(s.height, s.width, CV_32FC1,cv::Scalar::all(0));
	Mat cost=cv::Mat(s.height, s.width, CV_32FC1,cv::Scalar::all(0));
	img->scanline(1.0,3.0,15, disp, cost);
	std::cout << "Execution time:  " << double( clock() - tStart) / (double)CLOCKS_PER_SEC<< " seconds." << std::endl;
	double minv, maxv;
	cv::Point minL, maxL;
	cv::minMaxLoc(disp, &minv,&maxv);
	
	Mat disp8 = Mat(disp.size().height, disp.size().width, CV_8UC1, Scalar::all(0));
	disp.convertTo( disp8, CV_8UC1,255.0/(maxv-minv));
	 
   /* for (int i = 0; i < image_left.rows; i++)
	{
		for (int j = 0; j < image_left.cols ; j++)
		{
			cout<< "disp: " << disp.at<float>(i,j) << endl;
		}
		
	}*/
	
	cout<< disp8.type() << endl;
    imshow( "Disp", disp8 );                   	
    waitKey(0);
   
    /*
    unsigned a = 12;
    double b = (double)a;
  //  double c = fabs((double) b-a)/2;
    printf ("b: %f\t , a: %u\t\n " , b, a);
    */
	char c = waitKey(10);
    if (c == ' ')  
		return 0;
}

