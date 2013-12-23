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
	if(argc !=3){
		cerr << "Usage: ./dilate image iter" << endl;
		return -1;
	}
	src = imread(argv[1] , 0);
	itr = atoi(argv[2]);
	char* fullname = argv[1];
	char* bname = basename(fullname);
	string fname = reinterpret_cast<char*>(bname);
	//cerr << "fname: " <<  fname << endl;

	Mat dst1;
	dilate(src, dst1, Mat(), Point(-1,-1),itr);

	//imshow( "dilate" , dst1);
	string fpath = "/home/bahar/Master/stereo/gtfill/" + fname;
	imwrite( fpath , dst1);
	//waitKey(0);
	return 0;
}

