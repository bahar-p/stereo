/*
 * calibration.cpp
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

using namespace std;
using namespace cv;


void takeImage();
int takeVideo();
void readCalibfile(string, Mat*, Mat*);
enum mode {IMAGE , VIDEO};
Mat cameraMatrixL, distCoeffsL, cameraMatrixR, distCoeffsR;

int main(int argc, char **argv)
{ 
	
	mode option;
	char instring[7];
	printf("Do you want to capture IMAGE or VIDEO?	");
	scanf("%s",instring);
	while(strcmp(instring,"IMAGE")!=0 && strcmp(instring, "image")!=0 && strcmp(instring,"VIDEO")!=0 && strcmp(instring,"video")!=0){
		cout<< "Invalid input option! Please select either IMAGE or VIDEO"<<endl;
		printf("Do you want to capture IMAGE or VIDEO?	");
		scanf("%s",instring);
	}
	if(strcmp(instring,"IMAGE")==0 || strcmp(instring, "image")==0)
		option=IMAGE;
	else
		option=VIDEO;
		
	switch(option){
		case IMAGE:
			cout<<"taking image"<<endl;
			takeImage();
		break;
			
		case VIDEO:
			cout<<"taking Video"<<endl;
			takeVideo();
		break;
		
		default:
			cout<<"default"<<endl;
		break;
		
	}
	
	
   return 0;
}



void takeImage(){
	CvCapture* captureL = cvCaptureFromCAM(1); //right
	CvCapture* captureR = cvCaptureFromCAM(0); //mid
	CvCapture* captureM = cvCaptureFromCAM(2); //mid
	//CvCapture* captureUp = cvCaptureFromCAM(2); //right
	
	int img_num=30;
	string filenameL[img_num];
	string filenameR[img_num];
	//string filenameU[img_num];
	for (int i = 0; i < img_num; i++)
	{
		stringstream ss;
		ss << i;
		filenameL[i]=ss.str();
		filenameL[i]+="right.jpg";
		filenameR[i]=ss.str();
		filenameR[i]+="mid.jpg";
		//filenameU[i]=ss.str();
		//filenameU[i]+="up.jpg";
	}
	
	if ( !captureL || !captureR || !captureM) {
     fprintf( stderr, "ERROR: capture is NULL \n" );
     getchar();
    // return -1;
   }
   // Create a window in which the captured images will be presented
   
   // Show the image captured from the camera in the window and repeat
   int n=0;
   while (n<=img_num) {
     // Get one frame
     IplImage* frameL = cvQueryFrame( captureL );
     IplImage* frameR = cvQueryFrame( captureR );
     IplImage* frameM = cvQueryFrame( captureM );
     if ( !frameL || !frameR) {
       fprintf( stderr, "ERROR: frame is null...\n" );
       getchar();
       break;
     }
     char c = waitKey(10);
     if(c == ' '){
		cvSaveImage(filenameL[n].c_str(), frameL);
		cvSaveImage(filenameR[n].c_str(), frameR);
		//cvSaveImage(filenameU[n].c_str(), frameU);
		n++;
		cout << "n: " << n << endl;
	}
	cvShowImage( "windowL", frameL );
	cvShowImage( "windowR", frameR );
	cvShowImage( "windowM", frameM );
	//cvShowImage( "windowU", frameU );
	
     // Do not release the frame!
     //If ESC key pressed, Key=0x10001B under OpenCV 0.9.7(linux version),
     //remove higher bits using AND operator
     if ( (waitKey(10) & 255) == 27 ) break;
   }
   // Release the capture device housekeeping
   cvReleaseCapture( &captureL );
   cvReleaseCapture( &captureR );
   cvReleaseCapture( &captureM );
   //cvReleaseCapture( &captureUp );
   cvDestroyAllWindows();
}

/* Take stereo video and Save */
int takeVideo(){
   char dir[100];
   
   printf("Enter the path to the directory where you want to save the videos: ");
   scanf("%s",dir);
   string path = string(dir);
   cout << "path to directory: " << path << endl;
   // Open calibration file //
	//char calibFileL[100];
	//char calibFileR[100];

	/*printf("Enter left camera calibration file: ");
			scanf("%s",calibFileL);
			readCalibfile(calibFileL, &cameraMatrixL, &distCoeffsL);
			printf("Enter right camera calibration file: ");
			scanf("%s",calibFileR);
			readCalibfile(calibFileR, &cameraMatrixR, &distCoeffsR);*/
   
   VideoCapture cap1(3); // open the video camera no. 0 (right)
   VideoCapture cap2(0); // open the video camera no. 1 (mid)

    if (!cap1.isOpened() || !cap2.isOpened())  // if not success, exit program
    {
        cout << "ERROR: Cannot open the video file" << endl;
        return -1;
    }

	
			
   double dWidth = cap1.get(CV_CAP_PROP_FRAME_WIDTH); 			//get the width of frames of the video
   double dHeight = cap1.get(CV_CAP_PROP_FRAME_HEIGHT); 		//get the height of frames of the video
   cout << "Frame Size = " << dWidth << "x" << dHeight << endl;
   Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

	VideoWriter oVideoWriterL (path + "/CamMid.mpg", CV_FOURCC('P','I','M','1'), 20, frameSize, true); //initialize the VideoWriter object 
	VideoWriter oVideoWriterR (path + "/CamRight.mpg", CV_FOURCC('P','I','M','1'), 20, frameSize, true); //initialize the VideoWriter object 
	
	
   if ( !oVideoWriterL.isOpened() || !oVideoWriterR.isOpened()) //if not initialize the VideoWriter successfully, exit the program
   {
		cout << "ERROR: Failed to write the video" << endl;
		return -1;
   }
   
	/*Mat map1, map2, map3, map4;

	//Left stream
	initUndistortRectifyMap(cameraMatrixL, distCoeffsL, Mat(),
		  getOptimalNewCameraMatrix(cameraMatrixL, distCoeffsL, frameSize, 1, frameSize, 0),
		  frameSize, CV_16SC2, map1, map2);
	//Right stream
	initUndistortRectifyMap(cameraMatrixR, distCoeffsR, Mat(),
		  getOptimalNewCameraMatrix(cameraMatrixR, distCoeffsR, frameSize, 1, frameSize, 0),
		  frameSize, CV_16SC2, map3, map4);
	
	// Disparity parameters //
	int mindisp=0, maxdisp=64, SADWindow=9,dispMaxdiff=2;
	int P= 3*SADWindow;
	int preFilterCap = 31;
	int uniqueness = 10;
	int speckleWS = 100;
	int speckleRange =32;
	
	*/
	while (1)
	{
		// Apply undistortion to frames //
		Mat frameL, frameR;
		bool rightRead = cap1.read(frameR);
		bool leftRead = cap2.read(frameL);
		
		if (!leftRead || !rightRead) //if not success, break loop
		{
			 cout << "ERROR: Cannot read a frame from video file" << endl;
			 break;
		}
		//Mat GframeL, GframeR, uframeL, uframeR;
		
		//remap and show the rectified videos
		//cvtColor(frameL, GframeL, CV_BGR2GRAY);
		//cvtColor(frameR, GframeR, CV_BGR2GRAY);
		//remap(GframeL, uframeL, map1, map2, INTER_LINEAR);
		//remap(GframeR, uframeR, map3, map4, INTER_LINEAR);
		
		
		imshow("camLeft", frameL); 		//show the frame
		imshow("camMid", frameR); 	//show the frame
		
		
		//Store the video input in forms of .mpg files from both cameras
		oVideoWriterL.write(frameL); //writer the frame into the file
		oVideoWriterR.write(frameR); //writer the frame into the file
		
		/*Mat disp(dHeight,dWidth,CV_8U);
		//Apply SGBM and produce disparity map
		StereoSGBM sgbm(mindisp, maxdisp, SADWindow, 8*P, 32*P, dispMaxdiff,
                        preFilterCap, uniqueness, speckleWS, speckleRange, false);

		sgbm(uframeL,uframeR,disp);
		Mat disp8;
        disp.convertTo(disp8, CV_8U, 255/(maxdisp*16.));
		imshow("disparity", disp8);*/
		
		if (waitKey(10) == 27) //wait for 'esc' key press for 10ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}
	}

}

void readCalibfile (string filename, Mat* cameraMatrix, Mat* distCoeffs){
	FileStorage fs(filename, FileStorage::READ);
	fs["Camera_Matrix"] >> *cameraMatrix;
	fs["Distortion_Coefficients"] >> *distCoeffs;
	cout << "camera matrix: " << *cameraMatrix << endl
	<< "distortion coefficient: " << *distCoeffs << endl;
	fs.release();
}

