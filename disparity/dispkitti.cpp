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
#include <GL/glut.h>
#include <GL/gl.h>
#include <sstream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace cv;

#define PI 3.14159265

//void readCalibfile(string, string);
void kittiCalib(string);
Mat cameraMatrix[2], distCoeffs[2];
Mat T,R,R1,R2,P1,P2,Q;
Mat leftimg,rightimg,p1,p2, maskgt;
int dWidth,dHeight;
// Disparity parameters //
	int mindisp=0, maxdisp=160, SADWindow=9,dispMaxdiff=2;
	int P= 3*SADWindow;
	int preFilterCap = 31;
	int uniqueness = 10;
	int speckleWS = 100;
	int speckleRange =2;

int win_w = 800, win_h=800;
Mat Image3d;
std::mutex mtx;
std::condition_variable cvar;
bool filled;
bool rendered=true;
Mat glLeftf;
double LR_angle=271, UD_angle=5, ZOOM=3000.0;
int dragging, drag_x_origin, drag_y_origin;
bool PAUSE;
int frameNum;
Size frameSize;
void frames(int id) {
	
	Mat cimgL;
	cout << leftimg.type() << " " << leftimg.depth() << " " << leftimg.channels() <<  endl;
	Mat disp8;
	dWidth = leftimg.cols; 			//get the width of frames of the video
	dHeight = leftimg.rows; 		//get the height of frames of the video


	//cout << leftimg << endl;

	// Apply undistortion to frames //
//	Mat map[2][2];

/*	initUndistortRectifyMap(cameraMatrix[0], distCoeffs[0], NULL, P1, frameSize, CV_16SC2, map[0][0], map[0][1]); //left
	initUndistortRectifyMap(cameraMatrix[1], distCoeffs[1], NULL, P2, frameSize, CV_16SC2, map[1][0], map[1][1]); //right	
		
		Mat frameL, frameR;
		frameL=leftimg;
		frameR=rightimg;
		Mat GframeL, GframeR, uframeL, uframeR, newframeL;
		
		
		//remap and show the rectified videos
		cvtColor(frameL, GframeL, CV_BGR2GRAY);
		cvtColor(frameR, GframeR, CV_BGR2GRAY);
		remap(GframeL, uframeL, map[0][0], map[0][1], CV_INTER_LINEAR);
		remap(frameL, newframeL, map[0][0], map[0][1], CV_INTER_LINEAR);
		remap(GframeR, uframeR, map[1][0], map[1][1], CV_INTER_LINEAR);
		
		
		imshow("camLeft", uframeL); 		//show the frame
		imshow("camRight", uframeR); 	//show the frame
*/		
		//Apply SGBM and produce disparity map
		Mat disp(dHeight,dWidth,CV_8U);
		StereoSGBM sgbm(mindisp, maxdisp, SADWindow, 8*P, 32*P, dispMaxdiff,
                        preFilterCap, uniqueness, speckleWS, speckleRange, true);
		sgbm(leftimg,rightimg,disp);
		//cout << "VIDEOPROCESS: before: rendered: " << rendered << "filled: " << filled << endl;
		
		//std::unique_lock<std::mutex> lock(mtx);
		//cvar.wait(lock, []{return rendered;});
		
		//cout << "VIDEOPROCESS: after: rendered: " << rendered << "filled: " << filled << endl;
	//	Image3d = Mat(disp.size().height, disp.size().width, CV_32FC3,Scalar::all(0));
	//	reprojectImageTo3D(disp,Image3d, Q, true,CV_32F);
		
        	disp.convertTo(disp8, CV_8U, 255/(maxdisp*16.));
        
		Mat dmasked;
		disp8.copyTo(dmasked, maskgt);
		//rightimg.copyTo(rightmsk, maskR);
		
		imshow("maskgt" , maskgt);
		imshow("disparity", dmasked);
		imshow("disp8", disp8);
		imwrite("/home/bahar/Dataset/mydisp/disp.png" , disp8);
		imwrite("/home/bahar/Dataset/mydisp/dmasked.png" , dmasked);
		waitKey(0);
		//imwrite( "/home/bahar/FrameDisp5.png", disp8 );
		if (waitKey(10) == 27) 				//wait for 'esc' key press for 10ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			return;
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
	gluLookAt(camX+200,camY+500,-camZ-1000, 0,0,-1000, 0.0,1.0,0.0);
	
	
	
	//cout << "RENDER: before: rendered: " << rendered << " filled: " << filled << endl;
	std::unique_lock<std::mutex> lock(mtx);
	
	cvar.wait(lock, []{return filled;});
	rendered = false;
	//cout << "RENDER: after: rendered: " << rendered << " filled: " << filled << endl;
	
	glBegin(GL_POINTS);
	
	for (int j = 0; j < dHeight ; j++){
		for (int i= 0; i < dWidth; i++){
			
			if (Image3d.at<Vec3f>(j,i)[2]>0) { 
				glColor3ub(glLeftf.at<Vec3b>(j,i)[2],glLeftf.at<Vec3b>(j,i)[1],glLeftf.at<Vec3b>(j,i)[0]);
				//cout << Image3d.at<Vec3f>(j,i)[0] << " " << Image3d.at<Vec3f>(j,i)[1] << " " << Image3d.at<Vec3f>(j,i)[2] << endl;
				glVertex3f( (int)Image3d.at<Vec3f>(j,i)[0],-(int)Image3d.at<Vec3f>(j,i)[1], -Image3d.at<Vec3f>(j,i)[2]);
				//glVertex3f( imgCol-i,imgRow-j, Image3d.at<Vec3f>(j,i)[2]);
			}
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
	//if(bt==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
		//exit(0);
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

/*---Keyboard function to change view position----*/
void keyboard_press(unsigned char key, int x, int y){
	//Exit program when "Q" or "q" key is pressed on keyboard
	if(key== 'Q' | key== 'q')
		exit(0);
		
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
	glutKeyboardFunc(keyboard_press);
	glutMainLoop();
	
}


int main(int argc, char **argv)
{
	if (argc !=5){
		cout<< "usage: ./disparity calibFile leftimg rightimg Mask";
		return 0;
	}

	p1= Mat(3,4,CV_64F);
	p2 = Mat(3,4,CV_64F);
	kittiCalib(argv[1]);
	
	leftimg = imread(argv[2],0);
	rightimg = imread(argv[3],0);
	maskgt = imread(argv[4],0);
	


	cout << "Read both images successfully" << endl;
	if(!leftimg.data || !rightimg.data){
		cerr << " No valid image" << endl;
		return -1;
	}
	
	/*
	// Load stereo video //
	char* dir;
	printf("\nEnter right camera file path: ");
	scanf("%s",dir);
	string path = string(dir);
	cout << path << endl;
	leftimg = imread(dir,CV_LOAD_IMAGE_GRAYSCALE);
	//VideoCapture cap1(path); // open the video camera no. 0 (Right)
	//glcap1 = cap1;

	if(!leftimg.data){
		cout << "NO image data!" <<endl;
		return -1;
	}
	frameSize=leftimg.size();
	
	printf("\nEnter left camera file path: ");
	scanf("%s",dir);
	path = string(dir);
	cout << path << endl;
	
	rightimg = imread(dir,CV_LOAD_IMAGE_GRAYSCALE);
	if(!rightimg.data){
		cout << "NO image data!" <<endl;
		return -1;
	}*/

	//namedWindow( "left", CV_WINDOW_AUTOSIZE );
	//VideoCapture cap2(path); // open the video camera no. 1 (Left)
	//glcap2 = cap2;
	
//	glutInit(&argc, argv);
	
	std::thread camProcess(frames, 1);
	//std::thread graphics(draw, 2);
	
	camProcess.join();
	//graphics.join();
	
	return 0;
}


void kittiCalib(string calib){
	string l1,l2;
	ifstream infile;
	infile.open (calib);
	infile.ignore(4, ' ');
	getline(infile,l1);
	char * line1 =(char *) l1.c_str();
	cout << line1 << endl;
	char* p = strtok(line1, " ");
	int i=0, j=0;
	while(p!=NULL){	
		cout << p << endl;
		stringstream ss(p);
		ss >> p1.at<float>(i,j);
		j++;
		if(j==2){
			i++;
			j=0;
		}
		p = strtok(NULL, " ");
	}
	cout << "P1: " << p1 << endl;
	Mat roi1 = p1(Rect(0,0,3,3));
	cameraMatrix[0] = roi1;


	infile.ignore(4, ' ');
	getline(infile,l2);
	char * line2 =(char *) l2.c_str();
	cout << line2 << endl;
	p = strtok(line2, " ");
	i=0; j=0;
	while(p!=NULL){	
		cout << p << endl;
		stringstream ss2(p);
		ss2 >> p2.at<float>(i,j);
		j++;
		if(j==2){
			i++;
			j=0;
		}
		p = strtok(NULL, " ");
	}
	cout << "P2: " << p2 << endl;	
	Mat roi2 = p2(Rect(0,0,3,3));
	cameraMatrix[1] = roi2;
	cout << "closing the file" << endl;	
	infile.close();
	cout << "file closed" << endl;
	/*float tx = p2.at<float>(0,3) / p2.at<float>(0,0);
	cout << "tx: " << tx << endl;
	Mat R = (Mat_<float>(3,3) << 1,0,0,0,1,0,0,0,1);
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
