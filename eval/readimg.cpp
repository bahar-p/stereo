#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <fstream>

int main(int argc, char** argv){
	cv::Mat disp = cv::imread(argv[1],-1);
	int dmax = atoi(argv[2]);
	int adc = atoi(argv[3]);
	cv::Mat fdisp;
	if(adc) disp.convertTo(fdisp, CV_32F, dmax/(16*255.));
	else disp.convertTo(fdisp, CV_32F, dmax/255.);
	std::cout << fdisp(cv::Rect(100,100,10,10)) << std::endl;
	return 0;


}
