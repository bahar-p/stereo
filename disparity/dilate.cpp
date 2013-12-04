#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "cv.h"
#include "highgui.h"

using namespace std;
using namespace cv;

int main (int argc, char* argv[]){

	int itr;
	Mat src;
	if(argc<3){
		cerr << "Usage: ./dilate image iter";
		return -1;
	}
	src = imread(argv[1] , 0);
	itr = atoi(argv[2]);
	cerr << "itr: " << itr << endl;

	Mat dst1,dst2;
	dilate(src, dst1, Mat(), Point(-1,-1),itr);
	erode(dst1, dst2, Mat(), Point(-1,-1),itr);

	imshow( "dilate" , dst1);
	imshow( "erode" , dst2);
	imwrite( "/home/bahar/Dataset/gtfilled/gt.png" , dst2);
	waitKey(0);
	return 0;
}

