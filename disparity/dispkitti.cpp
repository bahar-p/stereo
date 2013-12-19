/*
 * disparity.cpp
 * 
 * Copyright 2013 Bahar <bahar@Bahar-PC>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "cv.h"
#include "highgui.h"
#include <sstream>

using namespace std;
using namespace cv;

//void readCalibfile(string, string);
void kittiCalib(string);
Mat cameraMatrix[2], distCoeffs[2];
Mat T,R,R1,R2,P1,P2,Q;
Mat leftimg,rightimg,p1,p2, mask;
int dWidth,dHeight;
string fname;
Size frameSize;
// Disparity parameters //

int main(int argc, char **argv)
{
	if (argc<5){
		cout<< "usage: ./disparity calibFile leftimg rightimg maxdisp ?mask?" << endl;
		return 0;
	}

	int mindisp=0, maxdisp, SADWindow=9,dispMaxdiff=2;
	int P= 3*SADWindow;
	int preFilterCap = 31;
	int uniqueness = 10;
	int speckleWS = 100;
	int speckleRange =2;
	
	cv::Mat mask;
	p1= Mat(3,4,CV_64F);
	p2 = Mat(3,4,CV_64F);
	kittiCalib(argv[1]);	
	leftimg = imread(argv[2],0);
	rightimg = imread(argv[3],0);
	maxdisp = atoi(argv[4]);
	if(argc==6) {
		mask = imread(argv[5],0);
	}
	char* fullpath = argv[2];
	char* bname = basename(fullpath);
	char* x = strtok(bname, ".");
	fname = (reinterpret_cast<char*>(bname));

	if(!leftimg.data || !rightimg.data){
		cerr << " No valid image" << endl;
		return -1;
	}
	
	//cout << leftimg.type() << " " << leftimg.depth() << " " << leftimg.channels() <<  endl;
	Mat disp8;
	dWidth = leftimg.cols; 			//get the width of frames of the video
	dHeight = leftimg.rows; 		//get the height of frames of the video

	clock_t tStart = clock();
	Mat disp(dHeight,dWidth, CV_8U, Scalar::all(0));
	StereoSGBM sgbm(mindisp, maxdisp, SADWindow, 8*P, 32*P, dispMaxdiff,
                        preFilterCap, uniqueness, speckleWS, speckleRange, true);
	sgbm(leftimg,rightimg,disp);
	cout << "SGBM Execution_time: " << (double) (clock() - tStart) / (double)CLOCKS_PER_SEC << " secs " << endl;
	disp.convertTo(disp8, CV_8U, 255/(maxdisp*16.)); 
	Mat dmasked;

	if(argc==6){
		string fpath2 = "/home/bahar/Master/stereo/Ex1/sgbm/dispmasked/" + fname + ".png";
		disp8.copyTo(dmasked, mask);
		imwrite( fpath2 , dmasked);
		//imshow("mask" , mask);
		//imshow("disparity", dmasked);
	}
	
	string fpath1 = "/home/bahar/Master/stereo/Ex1/sgbm/mydisp/" + fname + ".png";
	imwrite(fpath1 , disp8);
	//imshow("disp8", disp8);
	
	//waitKey(0);
	//imwrite( "/home/bahar/FrameDisp5.png", disp8 );
	/*if (waitKey(10) == 27) 				//wait for 'esc' key press for 10ms. If 'esc' key is pressed, break loop
	{
		cout << "esc key is pressed by user" << endl;
		return 0;
	}*/
	return 0;
}


void kittiCalib(string calib){
	string l1,l2;
	ifstream infile;
	infile.open (calib);
	infile.ignore(4, ' ');
	getline(infile,l1);
	char * line1 =(char *) l1.c_str();
	//cout << line1 << endl;
	char* p = strtok(line1, " ");
	int i=0, j=0;
	while(p!=NULL){	
		//cout << p << endl;
		stringstream ss(p);
	//	cout << "f: " << f << endl;
		ss >> p1.at<double>(i,j);
	//	cout << "p1: " << p1.at<float>(i,j) << endl;
		j++;
		if(j==4){
			i++;
			j=0;
		}
		p = strtok(NULL, " ");
	}
	//cout << "P1: " << p1 << endl;
	Mat roi1 = p1(Rect(0,0,3,3));
	cameraMatrix[0] = roi1;


	infile.ignore(4, ' ');
	getline(infile,l2);
	char * line2 =(char *) l2.c_str();
	//cout << line2 << endl;
	p = strtok(line2, " ");
	i=0; j=0;
	while(p!=NULL){	
		//cout << p << endl;
		stringstream ss2(p);
		ss2 >> p2.at<double>(i,j);
		j++;
		if(j==4){
			i++;
			j=0;
		}
		p = strtok(NULL, " ");
	}
	//cout << "P2: " << p2 << endl;	
	Mat roi2 = p2(Rect(0,0,3,3));
	cameraMatrix[1] = roi2;
	infile.close();
	double f = p2.at<double>(0,0);
	double tf = p2.at<double>(0,3);
	double tx = tf / f;
	cout << " Translation: " << tx << " Focal length: " << f <<  endl;
	/*Mat R = (Mat_<float>(3,3) << 1,0,0,0,1,0,0,0,1);
	Mat T = (Mat_<float>(3,1) << tx,0,0 );
	distCoeffs[0] = Mat(1,8, CV_32F, Scalar::all(0));
	distCoeffs[1]= distCoeffs[0];
	Mat P11, P22;
	cout << "returning from kitticalib...." << endl;
	stereoRectify(cameraMatrix[0],distCoeffs[0],cameraMatrix[1],distCoeffs[1],frameSize, R,T,R1,R2,P11, P22,Q);
	cout << "R1: " << R1 << " R2: " << R2 << " " << " P11: " << P11 << " P22: " << P22 << endl; */
}

/*
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
	
}*/
