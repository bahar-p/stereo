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

	dilate(src, src, Mat(), Point(-1,-1),itr);

	imshow( "dilate" , src);
	imwrite( "/home/bahar/Dataset/gtfilled/gt.png" , src );
	waitKey(0);
	return 0;
}

