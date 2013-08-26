#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

class image
{
private:
#define subRW 4
#define subRH 3
#define mytype CV_64FC1
cv::Mat DSI,init_cost,aggr_cost,final_cost,left_cost,right_cost, up_cost, down_cost, HII, VII, sumH, sumV, supReg;
uint64_t** censusLeft;
uint64_t** censusRight;
unsigned*** census_hamming;
cv::Mat img_leftRGB, img_rightRGB,disp,cost;
int dispMin, dispMax;
int img_rows, img_cols;

public:
image(cv::Mat, cv::Mat, int , int);
void read_image();
cv::Mat get_image(int);
void costAD();
void costCensus(int, int, int);
void hamdist(uint64_t**, uint64_t** , int , int );
void c_census(int, int);
void initCost(double , double);
void line_segment(double,double,double,double);
int colDiffer (cv::Mat, int, int, int, int);
void aggregateCost( );
double costOpt(cv::Mat, int, int, int, double, char, double, double, double);
double MinPathCost(cv::Mat, int, int);
cv::Mat scanline(double, double, double);
std::pair<double,double> calc_param(int, int, int, int, int, int ,int , int, double, double, double);
void IImage(cv::Mat, cv::Mat, char);
void finalSum(cv::Mat , cv::Mat, char, int a=0);
double minimum(double, double, double, double val=-1);
bool checkDisp(int, bool);
void find_disparity(cv::Mat, cv::Mat, cv::Mat);
void finalCost(cv::Mat , cv::Mat , cv::Mat , cv::Mat , cv::Mat );
double findMax(cv::Mat);
void subpxEnhance(cv::Mat, cv::Mat);

};
