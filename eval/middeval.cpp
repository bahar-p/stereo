#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <fstream>

//Global Variables

//float* dispErrorAvg(cv::Mat& d_gt, cv::Mat& d_gen, int dmax, bool mask);
bool isValid(float d){
	bool valid = (d > 0 ? true : false);
	return valid;
}
bool mydValid(float d){
	bool valid = (d >= 0 ? true : false);
	return valid;
}

int main (int argc, char** argv) {
	int adcensus=0;
	if(argc<5) {
		std::cerr << "Usage: ./eval gt_disp gen_disp maxDisp adcen" << std::endl;
		return -1;
	}
	cv::Mat im_gt = cv::imread(argv[1],-1);
	cv::Mat im_gen = cv::imread(argv[2],-1);
	int dmax = atoi(argv[3]);	
	adcensus = atoi(argv[4]);	
	if(!im_gt.data || !im_gen.data){
		std::cerr << "Invalid Image" << std::endl;
		return -1;
	}
	
	cv::Mat d_gt, d_gen;
	im_gt.convertTo(d_gt, CV_32F, 1/16.);
	//d_gt = im_gt;
	if(adcensus) im_gen.convertTo(d_gen, CV_32F,dmax/(16.*255)); 		//if it's AdCensus generated disparity
	else im_gen.convertTo(d_gen, CV_32F,dmax/(255.)); 			//if it's sgbm generate disparity
	int height = d_gt.size().height;
	int width = d_gt.size().width;
	
	float err_thr[4]={0.5,0.75,1,2};		//Different error thresholds
	int arr_len = 4;
	//int arr_len = 10;
	float err_total_pxs[arr_len];			//Number of incorrect pixels corresponding to each threshold
	float err_valid_pxs[arr_len];			//Number of incorrect valid pixels in the result corresponding to each threshold
	float density =0;				//density of disparity results
	for(int i=0;i<arr_len ;i++){
		err_total_pxs[i]=0;
		err_valid_pxs[i]=0;
	}
	int pix_count = 0;				//Total number of valid pixels in ground truth
	int pix_gen_count = 0;		
	
//	std::cout << "sampleGT: " << d_gt(cv::Rect(80,80,15,15)) << std::endl;
//	std::cout << "generated_disp: " << d_gen(cv::Rect(80,80,15,15)) << std::endl;
	float INF = std::numeric_limits<float>::infinity();
	
	/***** Disp Error Outliers *****/
	for(int j=0; j<height;j++){
		for(int i=dmax; i<width; i++){
			float fd_gt = d_gt.at<float>(j,i);
			float fd_gen = d_gen.at<float>(j,i);
			if(isValid(fd_gt)) {
				pix_count++;
				if(fd_gen < 0) std::cout << "Negative value!" << std::endl;
				float err = fabs(fd_gt - (fd_gen < 0 ? 0 : fd_gen));
				//std::cout << "disp_err: " << err << "  depth: " << depth_err << "  dz_1729: "  << dz_1729 << " dz_3049: " << dz_3049 << std::endl;
				//Write to output file
				
				for(int k=0;k<4;k++){
					if(err > err_thr[k])
						err_total_pxs[k]++;
				}
				if(mydValid(fd_gen)){
					pix_gen_count++;
					for(int k=0;k<4;k++){
						if(err > err_thr[k])
							err_valid_pxs[k]++;
					}
				}
			}
		}
	}

	if(pix_count==0){
		std::cout << "fatal error: there's no valid disparity in grount truth!!" << std::endl;
		return -2;
	}

	//normalize
	for(int i=0;i<arr_len;i++){
		err_total_pxs[i] /= std::max((float)pix_count, 1.0f);
		if(pix_gen_count > 0)
			err_valid_pxs[i] /= std::max((float)pix_gen_count,1.0f);
		std::cout <<  "outliers for all and just valid disp results for threshold " <<  err_thr[i]  << " are: " << 
		err_total_pxs[i] << "  " <<  err_valid_pxs[i] << std::endl;
	}
	density = (float)pix_gen_count/std::max((float)pix_count,1.0f);
	std::cout << "total valid pixs in gt: " << pix_count << " number of valid disp results in gen_disp: " << 
	pix_gen_count << " density of generated results: " << density << std::endl;

return 0;
}
