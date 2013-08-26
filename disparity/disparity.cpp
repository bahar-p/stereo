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
#include "cv.h"
#include "highgui.h"
#include <sstream>
#include <thread>

using namespace std;
using namespace cv;

void readCalibfile(string, string);
Mat cameraMatrix[2], distCoeffs[2];
Mat T,R,R1,R2,P1,P2,Q;
VideoCapture glcap1, glcap2;

void frameProcess(VideoCapture&, VideoCapture&);

void frameProcess(VideoCapture& c1, VideoCapture& c2){
	
	
}


int main(int argc, char **argv)
{
	if (argc <=1 ){
		cout<< "usage: ./disparit extrinsic_file instrinsic_file ";
		return 0;
	}
	
	readCalibfile(argv[1], argv[2]);
	// Load stereo video //
	char dir[100];
	printf("Enter right camera file path: ");
	scanf("%s",dir);
	string path = string(dir);
	cout << path << endl;
	VideoCapture cap1(path); // open the video camera no. 0 (Right)
	glcap1 = cap1;
	
	printf("Enter left camera file path: ");
	scanf("%s",dir);
	path = string(dir);
	cout << path << endl;
	VideoCapture cap2(path); // open the video camera no. 1 (Left)
	glcap2 = cap2;
	
	if (!cap1.isOpened() || !cap2.isOpened())  // if not success, exit program
	{
		cout << "ERROR: Cannot open the video file" << endl;
		return -1;
	}
	
	
	double dWidth = cap1.get(CV_CAP_PROP_FRAME_WIDTH); 			//get the width of frames of the video
	double dHeight = cap1.get(CV_CAP_PROP_FRAME_HEIGHT); 		//get the height of frames of the video
	cout << "Frame Size = " << dWidth << "x" << dHeight << endl;
	Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));
   
   // Apply undistortion to frames //
	Mat map[2][2];

	initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1, frameSize, CV_16SC2, map[0][0], map[0][1]); //left
    initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2, frameSize, CV_16SC2, map[1][0], map[1][1]); //right
	
	
	// Disparity parameters //
	int mindisp=0, maxdisp=64, SADWindow=9,dispMaxdiff=2;
	int P= 3*SADWindow;
	int preFilterCap = 31;
	int uniqueness = 10;
	int speckleWS = 100;
	int speckleRange =32;
	
	while (1) {
		Mat frameL, frameR;
		bool rightRead = cap1.read(frameR);
		bool leftRead = cap2.read(frameL);
		
		if (!leftRead || !rightRead) //if not success, break loop
		{
			 cout << "ERROR: Cannot read a frame from video file" << endl;
			 break;
		}
		
		
		Mat GframeL, GframeR, uframeL, uframeR;
		Mat disp(dHeight,dWidth,CV_8U);
		//remap and show the rectified videos
		cvtColor(frameL, GframeL, CV_BGR2GRAY);
		cvtColor(frameR, GframeR, CV_BGR2GRAY);
		remap(GframeL, uframeL, map[0][0], map[0][1], CV_INTER_LINEAR);
		remap(GframeR, uframeR, map[1][0], map[1][1], CV_INTER_LINEAR);
		
		
		imshow("camLeft", uframeL); 		//show the frame
		imshow("camRight", uframeR); 	//show the frame
		
		//Apply SGBM and produce disparity map
		StereoSGBM sgbm(mindisp, maxdisp, SADWindow, 8*P, 32*P, dispMaxdiff,
                        preFilterCap, uniqueness, speckleWS, speckleRange, false);

		sgbm(uframeL,uframeR,disp);
		Mat disp8;
        disp.convertTo(disp8, CV_8U, 255/(maxdisp*16.));
		imshow("disparity", disp8);
		
		if (waitKey(10) == 27) 				//wait for 'esc' key press for 10ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}
		
	}
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
	<< "R1: " << R1 << endl << "Q: " << Q << endl; 
	fse.release();
	fsi.release();
	
}


