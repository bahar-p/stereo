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
#include <GL/glut.h>
#include <GL/gl.h>
#include <sstream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace cv;

#define PI 3.14159265

void readCalibfile(string, string);
Mat cameraMatrix[2], distCoeffs[2];
Mat T,R,R1,R2,P1,P2,Q;
VideoCapture glcap1, glcap2;
// Disparity parameters //
	int mindisp=0, maxdisp=64, SADWindow=9,dispMaxdiff=2;
	int P= 3*SADWindow;
	int preFilterCap = 31;
	int uniqueness = 10;
	int speckleWS = 100;
	int speckleRange =32;

int win_w = 800, win_h=800;
Mat Image3d;
int imgRow, imgCol;
std::mutex mtx;
std::condition_variable cvar;
bool filled;
bool rendered=true;
Mat glLeftf;
double LR_angle=271, UD_angle=0, ZOOM=3000.0;
int dragging, drag_x_origin, drag_y_origin;

void frames(int id){
	
	
	double dWidth = glcap1.get(CV_CAP_PROP_FRAME_WIDTH); 			//get the width of frames of the video
	double dHeight = glcap1.get(CV_CAP_PROP_FRAME_HEIGHT); 		//get the height of frames of the video
	double fps = glcap1.get(CV_CAP_PROP_FPS);
	cout << "fps: " << fps << " Frame Size = " << dWidth << "x" << dHeight << endl;
	Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));
	imgRow = dHeight;
	imgCol = dWidth;
   // Apply undistortion to frames //
	Mat map[2][2];

	initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], R1, P1, frameSize, CV_16SC2, map[0][0], map[0][1]); //left
    initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], R2, P2, frameSize, CV_16SC2, map[1][0], map[1][1]); //right
	while (1) {
		filled=false;
		Mat frameL, frameR;
		bool rightRead = glcap1.read(frameR);
		bool leftRead = glcap2.read(frameL);
		
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
		//cout << "VIDEOPROCESS: before: rendered: " << rendered << "filled: " << filled << endl;
	
		std::unique_lock<std::mutex> lock(mtx);
		cvar.wait(lock, []{return rendered;});
		
		//cout << "VIDEOPROCESS: after: rendered: " << rendered << "filled: " << filled << endl;
		Image3d = Mat(disp.size().height, disp.size().width, CV_32FC3,Scalar::all(0));
		reprojectImageTo3D(disp,Image3d, Q, true,CV_32F);
		
		
		Mat disp8;
        disp.convertTo(disp8, CV_8U, 255/(maxdisp*16.));        
		imshow("disparity", disp8);
		
		if (waitKey(10) == 27) 				//wait for 'esc' key press for 10ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break; 
		}
		filled=true;
		glLeftf = frameL;
		cvar.notify_one();
	}
	
}
void init(){
	glClearColor(0.0, 0.0 , 0.0, 0.0);
	glViewport(0,0,win_w,win_h);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(0.0,10.0,0.0,10.0,-1.0,1.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
}
void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	//convert angles to radian
	double UD_rd= (UD_angle*PI)/180;
	double LR_rd= (LR_angle*PI)/180;
	double camX= cos(UD_rd)*ZOOM*cos(LR_rd);
	double camY= sin(UD_rd)*ZOOM;
	double camZ= cos(UD_rd)*ZOOM*sin(LR_rd);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-1, 10, -1,10, -1.0, 1.0);
	gluPerspective(30.0, (GLfloat)win_w/(GLfloat)win_h, 1.0,8000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camX-100,camY-200,camZ, -imgCol/2,-imgRow/2,400, 0.0,1.0,0.0);
	
	
	
	//cout << "RENDER: before: rendered: " << rendered << " filled: " << filled << endl;
	std::unique_lock<std::mutex> lock(mtx);
	
	cvar.wait(lock, []{return filled;});
	rendered = false;
	//cout << "RENDER: after: rendered: " << rendered << " filled: " << filled << endl;
	
	glBegin(GL_POINTS);
	
	for (int j = 0; j < imgRow ; j++){
		for (int i= 0; i < imgCol; i++){
			glColor3ub(glLeftf.at<Vec3b>(j,i)[2],glLeftf.at<Vec3b>(j,i)[1],glLeftf.at<Vec3b>(j,i)[0]);
			//cout << Image3d.at<Vec3f>(j,i)[0] << " " << Image3d.at<Vec3f>(j,i)[1] << " " << Image3d.at<Vec3f>(j,i)[2] << endl;
			//glVertex3f( -Image3d.at<Vec3f>(j,i)[0],-Image3d.at<Vec3f>(j,i)[1],Image3d.at<Vec3f>(j,i)[2]);
			glVertex3f( -i,-j, Image3d.at<Vec3f>(j,i)[2]);
		}
	}
	glEnd();
	rendered = true;
	//glutWireCube(1.0);
	glFlush(); 
	glutSwapBuffers();
	glutPostRedisplay();
	cvar.notify_one();
}
void reshape(int w, int h){
	win_w = w;
	win_h = h;
	glViewport(0,0, (GLsizei) w, (GLsizei) h);
	
}

void mouse_press(int bt, int state, int i, int j){
	//Exit program on Mouse_Right_Button press
	if(bt==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
		exit(0);
	if (bt == 3) 		//mouse wheel event
	{
		ZOOM-=20;
	}
	if(bt ==4){			//mouse wheel event
	   ZOOM+=20;
	}
	if(bt==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
		dragging=1;
		drag_x_origin = i;
		drag_y_origin = j;
	}
	else 
		dragging=0;
}

void SpecialKeys(int key, int x, int y){
	
	if(key==GLUT_KEY_LEFT)
		LR_angle +=1;
		
	if(key == GLUT_KEY_RIGHT)
		LR_angle -=1;
		
	if(key==GLUT_KEY_UP)
		UD_angle +=1;
		
	if(key==GLUT_KEY_DOWN)
		UD_angle -=1;
	
}
void mouse_move(int x, int y){
	if(dragging){
		LR_angle += (x-drag_x_origin) * 0.2;
		UD_angle += (y-drag_y_origin)* 0.2;
		drag_x_origin = x;
		drag_y_origin = y;
		
	}
	
}

void draw(int id){
	
	cout << " \ngraphics" <<endl;
	
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Model");
	init();
	
	glutDisplayFunc(display);
	cout << "id: " << id <<endl;
	glutMouseFunc(mouse_press);
	glutSpecialFunc(SpecialKeys);
	glutReshapeFunc(reshape);
	glutMotionFunc(mouse_move);
	glutMainLoop();
	
}


int main(int argc, char **argv)
{
	if (argc <=1 ){
		cout<< "usage: ./disparit extrinsic_file instrinsic_file ";
		return 0;
	}
	
	readCalibfile(argv[1], argv[2]);
	// Load stereo video //
	/*char dir[100];
	printf("Enter right camera file path: ");
	scanf("%s",dir);
	string path = string(dir);
	cout << path << endl;*/
	string path = "../Day01/Office01R.mpg";
	VideoCapture cap1(path); // open the video camera no. 0 (Right)
	glcap1 = cap1;
	
	/*printf("Enter left camera file path: ");
	scanf("%s",dir);
	path = string(dir);
	cout << path << endl;*/
	path = "../Day01/Office01L.mpg";
	VideoCapture cap2(path); // open the video camera no. 1 (Left)
	glcap2 = cap2;
	
	if (!cap1.isOpened() || !cap2.isOpened())  // if not success, exit program
	{
		cout << "ERROR: Cannot open the video file" << endl;
		return -1;
	}

	glutInit(&argc, argv);
	//Image3d = Mat(480, 640, CV_32FC3, Scalar::all(0));
	
	std::thread camProcess(frames, 1);
	std::thread graphics(draw, 2);
	
	camProcess.join();
	graphics.join();
	
	/*
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
		
	}*/
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


