#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "cv.h"
#include "highgui.h"
#include <GL/glut.h>
#include <GL/gl.h>

#define PI 3.14159265
//Global Vars//
cv::Mat img, face, disp, fdisp;
float foc, bl;
double LR_angle=270, UD_angle=0, ZOOM=500;
int dgt, win_w = 500, win_h=500;
int dragging, drag_x_origin, drag_y_origin;

void init(){
	glClearColor(0.0, 0.0 , 0.0, 0.0);
	glViewport(0,0,win_w,win_h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
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
	//std::cout << camX << " " << camY << " " << camZ << std::endl;
	glLoadIdentity();
	//glOrtho(-200, 200, -200,200, -10.0, 1000.0);
	gluPerspective(30.0, (GLfloat)win_w/(GLfloat)win_h, 1.0,8000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camX,camY,camZ-300, img.cols/2,img.rows/2,2000, 0.0,1.0,0.0);
	
	for(int j=0; j<img.rows; j++){
		for(int i=0; i<img.cols; i++){
			if(fdisp.at<float>(j,i) > 0){
				glColor3ub(img.at<cv::Vec3b>(j,i).val[2] , img.at<cv::Vec3b>(j,i).val[1] , img.at<cv::Vec3b>(j,i).val[0]);
				float z = (foc*bl)/fdisp.at<float>(j,i);
				//std::cout << img.at<cv::Vec3b>(j,i) << " " << z << std::endl;
				glBegin(GL_POINTS);
					glVertex3f(i-(img.cols/2),j-(img.rows/2),z);
				glEnd();
			}
		}
	}

	for(int j=0; j<face.rows; j++){
		for(int i=0; i<face.cols; i++){
			float z = (foc*bl)/(dgt-1);
			glPushMatrix();
			glTranslatef(20,20,0);
			glBegin(GL_POINTS);
				glVertex3f(i-(face.cols/2),j-(face.rows/2),z);
			glEnd();
			glPopMatrix();
		}
	}
	//glPushMatrix();
	//glutWireCube(20.0);
	//glPopMatrix();
	glFlush(); 
	glutSwapBuffers();
	glutPostRedisplay();
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
		ZOOM-=10;
	}
	if(bt ==4){			//mouse wheel event
	   ZOOM+=10;
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
		LR_angle -=1;
		
	if(key == GLUT_KEY_RIGHT)
		LR_angle +=1;
		
	if(key==GLUT_KEY_UP)
		UD_angle -=1;
		
	if(key==GLUT_KEY_DOWN)
		UD_angle +=1;
	
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

int main(int argc, char* argv[]){

	if(argc < 7){
		std::cout << "Usage: ./AR img layer disp foc baseline dmax dgt_fg adc"  << std::endl;
		return -1;

	}
	glutInit(&argc, argv);
	img = cv::imread(argv[1],1);
	face = cv::imread(argv[2],-1);
	disp = cv::imread(argv[3],-1);
	foc = atof(argv[4]);
	bl= atof(argv[5]);
	int dmax = atoi(argv[6]);
	dgt = atoi(argv[7]);
	int adc = atoi(argv[8]);
	if(adc) disp.convertTo(fdisp, CV_32F, dmax/(16*255.));
	else disp.convertTo(fdisp, CV_32F, dmax/255.);

	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize(win_w,win_h);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Model");
	init();	
	glutDisplayFunc(display);
	glutMouseFunc(mouse_press);
	glutSpecialFunc(SpecialKeys);
	glutReshapeFunc(reshape);
	glutMotionFunc(mouse_move);
	glutKeyboardFunc(keyboard_press);
	glutMainLoop();
	return 0;
}
