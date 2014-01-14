#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "cv.h"
#include "highgui.h"

using namespace std;
using namespace cv;

Mat src, dst;
int itr;
const int max_itr = 10;
char* window_name = "";

void DilateIter(int, void*){
	dilate(src, dst, Mat(), Point(-1,-1),itr);
	imshow( window_name, dst);
}

int main (int argc, char* argv[]){
	if(argc < 3 ){
		cerr << "Usage: ./dilate image is_noc ?iter?" << endl;
		return -1;
	}
	src = imread(argv[1] , 0);
	int noc = atoi(argv[2]);
	if(argc==4) itr = atoi(argv[3]);
	else itr=0;
	char* fullname = argv[1];
	char* bname = basename(fullname);
	string fname = reinterpret_cast<char*>(bname);
	
	//Create Window to display
	window_name = (char *) fname.c_str();
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	//Create Trackbar
	createTrackbar("Dilate iteration:" ,window_name, &itr, max_itr, DilateIter);
	//Callback function
	DilateIter(itr,0);
	waitKey(0);
	string fpath;
	ofstream myfile;
	if(noc){
		myfile.open("/home/bahar/Master/stereo/gtfill/dilItr.txt", std::ios::out | std::ios::app);
		fpath = "/home/bahar/Master/stereo/gtfill/disp_noc/" + fname;
	}
	else{
		fpath = "/home/bahar/Master/stereo/gtfill/disp_occ/" + fname;
	}
	myfile << "img: " << fname << " dil_itr: " << itr << endl;
	imwrite( fpath , dst);
	return 0;
}

