#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cv.h"
#include "highgui.h"
#include <GL/glut.h>
#include <GL/gl.h>

using namespace cv;
using namespace std;
Mat R,T,R1,R2,P1,P2,Q, Rleft, Rright;
Mat cameraMatrix[2], distCoeffs[2];
void readCalibfile (string, string);

int main( int argc, char** argv){

	if(argc!=5){
		cout<< "usage: ./rectify extrinsic_file intrinsic_file leftImg rightImg" << endl;
		return -1;
	}
	readCalibfile(argv[1], argv[2]);
	Mat left = imread(argv[3],0); 
	Mat right = imread(argv[4],0); 
	Size frameSize = left.size();
	Mat map[2][2];
	initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1, frameSize, CV_16SC2, map[0][0], map[0][1]); //left
	initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2, frameSize, CV_16SC2, map[1][0], map[1][1]); //right
	remap(left, Rleft, map[0][0], map[0][1], CV_INTER_LINEAR);
	remap(right, Rright, map[1][0], map[1][1], CV_INTER_LINEAR);
	imwrite("/home/bahar/rectL.png",Rleft);
	imwrite("/home/bahar/rectR.png",Rright);
	return 0;
}

void readCalibfile (string extr, string intr){
	
	FileStorage fse(extr, FileStorage::READ);
	FileStorage fsi(intr, FileStorage::READ);
	
	fse["R"] >> R;
	fse["T"] >> T;
	fse["R1"] >> R1;
	fse["R2"] >> R2;
	fse["P1"] >> P1;
	fse["P2"] >> P2;
	fse["Q"] >> Q;
	fsi["M1"] >> cameraMatrix[0];
	fsi["D1"] >> distCoeffs[0];
	fsi["M2"] >> cameraMatrix[1];
	fsi["D2"] >> distCoeffs[1];
	cout << "camera matrix: " << cameraMatrix[0] << endl
	<< "distortion coefficient: " << distCoeffs[1] << endl
	<< "R1: " << R1 << endl << "P1: " << P1 << endl; 
	fse.release();
	fsi.release();
	
}


