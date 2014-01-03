#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <list>
#include <omp.h>
#include "cuda.h"
#include "cuda_runtime.h"
#include <opencv2/gpu/gpu.hpp>
using namespace std;
using namespace cv;
using namespace cv::gpu;

static __global__ void costAD(cv::gpu::PtrStepSz<uchar3>, cv::gpu::PtrStepSz<uchar3>, cv::gpu::PtrStepSz<double>,int,int,int,int,int,int, bool);

class image
{
private:
#define subRW 0
#define subRH 0
#define mytype CV_64FC1
cv::Mat supReg;
cv::Mat* sumH, *semi_cost;
uint64_t** censusLeft;
uint64_t** censusRight;
unsigned*** census_hamming;
cv::Mat img_leftRGB, img_rightRGB;
int dispMin, dispMax;
int img_rows, img_cols;
int channels;
Size s;
public:
image(cv::Mat, cv::Mat, int , int);
void read_image();
cv::Mat get_image(int);
int costAD_caller(const cv::gpu::PtrStepSz<uchar3>&,const cv::gpu::PtrStepSz<uchar3>&, const cv::gpu::PtrStepSz<double>&, int, bool dispr=false);
void costCensus(int, int, int);
void hamdist(uint64_t**, uint64_t** , int , int,bool dispr=false );
void c_census(int, int,bool dispr=false);
void initCost(cv::Mat*, double , double);
void line_segment(double,double,double,double, bool dispr=false);
double colDiffer (cv::Mat, int, int, int, int);
void aggregateCost(cv::Mat*);
double costOpt(cv::Mat*, int, int, int, double, char, double, double, double, bool dispr=false);
double MinPathCost(cv::Mat*, int, int);
cv::Mat* scanline(double, double, double, Mat&, Mat&,bool dispr=false);
std::pair<double,double> calc_param(int, int, int, int, int, int ,int , int, double, double, double);
void IImage(cv::Mat*, cv::Mat*, char);
void finalSum(cv::Mat* , cv::Mat*, char, int a=0);
double minimum(double, double, double, double val=-100);
bool dispValid(int);
void find_disparity(cv::Mat*, cv::Mat&, cv::Mat&);
void finalCost(cv::Mat , cv::Mat , cv::Mat , cv::Mat , cv::Mat& );
double findMax(cv::Mat*);
void subpxEnhance(cv::Mat*, cv::Mat&);
int findOutliers(cv::Mat, cv::Mat, cv::Mat&, float, float);
void fMatrix(cv::Mat, cv::Mat, cv::Mat&,int, double, double);
void reset();
void regionVoting(cv::Mat&, cv::Mat&, int, double, int);
int mostVote(cv::Mat);
int labelOut(cv::Mat, float,float , float, float);
void border(cv::Mat, cv::Mat&);
void discAdjust(cv::Mat&, cv::Mat*, cv::Mat);
void interpolate(cv::Mat, cv::Mat&, cv::Mat);
//DEBUG
char *itob(uint64_t);
};