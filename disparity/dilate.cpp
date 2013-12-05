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
	if(argc<4){
		cerr << "Usage: ./dilate image iter imgName" << endl;
		return -1;
	}
	src = imread(argv[1] , 0);
	itr = atoi(argv[2]);
	stringstream ss;
	ss << argv[3] << ".png" ;
	string fname = ss.str();
	cerr << "fname: " <<  fname <<  " itr: " << itr << endl;

	Mat dst1;
	dilate(src, dst1, Mat(), Point(-1,-1),itr);

	imshow( "dilate" , dst1);
	string fpath = "/home/bahar/Dataset/gtfilled/" + fname;
	imwrite( fpath , dst1);
	waitKey(0);
	return 0;
}

