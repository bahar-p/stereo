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
cv::Mat img, face, disp, fdisp, ARimg;
float foc, bl;
double LR_angle=264, UD_angle=20, ZOOM=860;
int dgt, win_w = 700, win_h=500;
int dragging, drag_x_origin, drag_y_origin;
void capture ();

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
	gluLookAt(camX,camY,camZ-100, -100,300,500, 0.0,1.0,0.0);
	
	for(int j=0; j<img.rows; j++){
		for(int i=0; i<img.cols; i++){
			if(fdisp.at<float>(j,i) > 0){
				glColor3ub(img.at<cv::Vec3b>(j,i).val[2] , img.at<cv::Vec3b>(j,i).val[1] , img.at<cv::Vec3b>(j,i).val[0]);
				float z = (foc*bl)/fdisp.at<float>(j,i);
				//std::cout << img.at<cv::Vec3b>(j,i) << " " << z << std::endl;
				glBegin(GL_POINTS);
					glVertex3f(-i,win_h-j,z);
				glEnd();
			}
		}
	}

	float z = (foc*bl)/dgt;
	glPushMatrix();
	glTranslatef(-200,330,z);
       	//glColor3ub(8, 105,240);
	//glutSolidTeapot(25);
	glBegin(GL_TRIANGLES);
        	glColor3ub(8, 105,240);
	        glVertex3f(0, 0, 0);
		glVertex3f(45, 0, 0);
		glVertex3f(0, 50, 0);
	glEnd();
	glPopMatrix();

	/*for(int j=0; j<face.rows; j++){
		for(int i=0; i<face.cols; i++){
			float z = (foc*bl)/dgt;
			glPushMatrix();
			glTranslatef(-103,-103,0);
			glBegin(GL_POINTS);
				glVertex3f(-i,win_h-j,z);
			glEnd();
			glPopMatrix();
		}
	}*/
	//glPushMatrix();
	//glutWireCube(20.0);
	//glPopMatrix();
	glFlush(); 
	glutSwapBuffers();
	glutPostRedisplay();
	//std::cout << "UD_angle: " << UD_angle <<  " LR_angle: " << LR_angle << " ZOOM: " << ZOOM << std::endl;
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
	if(key== 'C' | key== 'c')
		capture();
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

void capture(){
	ARimg = cv::Mat(img.rows, img.cols, CV_8UC3);
	glPixelStorei(GL_PACK_ALIGNMENT, (ARimg.step & 3)?1:4);
        glPixelStorei(GL_PACK_ROW_LENGTH, ARimg.step/ARimg.elemSize());
	glReadPixels(200, 140, ARimg.cols, ARimg.rows, GL_BGR_EXT, GL_UNSIGNED_BYTE, ARimg.data);
	cv::Mat flipped(ARimg);
	cv::flip(ARimg, flipped, 0);
	cv::imwrite("/home/bahar/snapshot.png", ARimg);
}

int main(int argc, char* argv[]){

	if(argc < 7){
		std::cout << "Usage: ./AR img layer disp foc baseline dmax dgt_fg adc"  << std::endl;
		return -1;

	}
	glutInit(&argc, argv);
	img = cv::imread(argv[1],1);
	face = cv::imread(argv[2],1);
	disp = cv::imread(argv[3],0);
	foc = atof(argv[4]);
	bl= atof(argv[5]);
	int dmax = atoi(argv[6]);
	dgt = atoi(argv[7]);
	int adc = atoi(argv[8]);
	if(adc==1) disp.convertTo(fdisp, CV_32F, dmax/(16*255.));
	else if(adc==0) disp.convertTo(fdisp, CV_32F, dmax/255.);
	else disp.convertTo(fdisp, CV_32F, dmax/8.);

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
