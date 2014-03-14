#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <fstream>


int main (int argc, char** argv) {

	cv::Mat mskimg = cv::imread(argv[1],0);
	int pix=0;
	for(int j=0; j<mskimg.rows; j++) {
		for(int i=0; i<mskimg.rows; i++) {
			if(mskimg.at<uchar>(j,i)!=0){
				pix++;
			}
		}
	}
	float ratio = (float)pix/(mskimg.rows*mskimg.cols);
	std::cout << "down sample ration: " << ratio << std::endl;

}
