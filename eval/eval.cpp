#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"

//Global Variables
cv::Mat mask;

float* dispErrorAvg(cv::Mat& d_gt, cv::Mat& d_gen, int dmax, bool mask);
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
	if(argc<6) {
		std::cerr << "Usage: ./eval gt_disp gen_disp maxDisp focal baseline ?Adcensusflag? ?mask?" << std::endl;
		return -1;
	}
	cv::Mat im_gt = cv::imread(argv[1],-1);
	cv::Mat im_gen = cv::imread(argv[2],-1);
	int dmax = atoi(argv[3]);
	float f = atof(argv[4]);
	float bs = atof(argv[5]);
	std::cout << "F: " << f << " BS: " << bs << std::endl;
	if(argc>6) adcensus=atoi(argv[6]);
	bool masked = false;
	if(argc>7) {
		mask = cv::imread(argv[7],0);
		masked = true;
	}
	if(!im_gt.data || !im_gen.data){
		std::cerr << "Invalid Image" << std::endl;
		return -1;
	}
	//Get the original disp values
	cv::Mat d_gt, d_gen;
	im_gt.convertTo(d_gt, CV_32F, 1/256.);
	if(adcensus) im_gen.convertTo(d_gen, CV_32F,dmax/(16.*255)); 		//if it's AdCensus generated disparity
	else im_gen.convertTo(d_gen, CV_32F,dmax/(255.)); 			//if it's sgbm generate disparity

	int height = d_gt.size().height;
	int width = d_gt.size().width;
	float err_thr[10]={0.3,0.6,0.7,1.5,1.5,1.0,1.4,1.2,4.0,4.5};		//Different error thresholds
	int arr_len = sizeof(err_thr)/sizeof(*err_thr);
	//int arr_len = 10;
	float err_total_pxs[arr_len];			//Number of incorrect pixels corresponding to each threshold
	float err_valid_pxs[arr_len];			//Number of incorrect valid pixels in the result corresponding to each threshold
	float density =0;				//density of disparity results
	for(int i=0;i<arr_len ;i++){
		err_total_pxs[i]=0;
		err_valid_pxs[i]=0;
	}
	int pix_count = 0;	//Total number of valid pixels in ground truth
	int pix_gen_count = 0;	//Total number of valid pixels in generated disparity
	float stAcuity_1729=32;				//Avg Stereo Acuity 17-29 age = 32arcsec
	float stAcuity_3049=33.75;			//Avg Stereo Acuity 17-29 age = 33.75arcsec
	float stAcuity_5069=38.75;			//Avg Stereo Acuity 17-29 age = 38.75arcsec
	float stAcuity_7083=112.5;			//Avg Stereo Acuity 17-29 age = 112.5arcsec
	float pupil_dist = 64; 				//Avg value of Interpupillary distance (mm)
	float c = 3437.75; 				//constant

	//std::cout << "d_gen: " << d_gen(cv::Rect(500,150, 80,1)) << std::endl;
	//std::cout << "d_gt: " << d_gt(cv::Rect(500,150, 80,1)) << std::endl;
	if(height != d_gen.size().height || width != d_gen.size().width){
		std::cerr << "disparity image sizes don't match!" << std::endl;
		return -1;
	}
	
	/***** Disp Error Outliers *****/
	for(int j=0; j<height;j++){
		for(int i=dmax; i<width; i++){
			if(masked){
				if(mask.at<uchar>(j,i)==0) continue;
			}
			float fd_gt = d_gt.at<float>(j,i);
			float fd_gen = d_gen.at<float>(j,i);
			if(isValid(fd_gt)) {
				pix_count++;
				if(fd_gen < 0) std::cout << "Negative value!" << std::endl;
				float err = fabs(fd_gt - (fd_gen < 0 ? 0 : fd_gen));
				float depth = (f*bs)/fd_gt;
				float dz_1729 = ((depth*depth)*(stAcuity_1729/60))/(c*pupil_dist);
				float dz_3049 = ((depth*depth)*(stAcuity_3049/60))/(c*pupil_dist);
				float depth_err = (f*bs)/err;
			//	float dz_err = ((depth_err*depth_err)*stAcuity)/(c*pupil_dist);

				std::cout << "disp_err: " << err << "  depth: " << depth_err << "  dz_1729: "  << dz_1729 << std::endl;
				if(depth_err >= dz_1729){
					err_total_pxs[0]++;
				}
				else if(depth_err >= dz_3049){
					err_total_pxs[1]++;
				}
				/*else if(depth >= 5 && depth < 7){
					if(depth_err >= err_thr[2])  err_total_pxs[2]++;		//FIX LATER (the condition should probably change)
				}
				else if(depth >= 7 && depth < 11){
					if(depth_err >= err_thr[3])  err_total_pxs[3]++;		//FIX LATER (the condition should probably change)
				}*/
				else {
					//std::cout << "depth out of defined ranges" << std::endl;
				}
				//std::cout << "err: " << err << std::endl;
				/*for(int k=0; k<arr_len; k++){	
					if(err > err_thr[k]) err_total_pxs[k]++; 
				}*/	
				if(mydValid(fd_gen)){
					pix_gen_count++;
					if(depth_err >= dz_1729){
						err_total_pxs[0]++;
					}
					else if(depth_err >= dz_3049){
						err_total_pxs[1]++;
					}
					else {
						//std::cout << "depth out of defined ranges" << std::endl;
					}
					/*for(int k=0; k<arr_len; k++){		
						if(err > err_thr[k]) err_valid_pxs[k]++; 
					}*/
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
		std::cout <<  "outliers for all and just valid disp results for thresh " << err_thr[i] << " are: " << 
		err_total_pxs[i] << "  " <<  err_valid_pxs[i] << std::endl;
	}
	density = (float)pix_gen_count/std::max((float)pix_count,1.0f);
	std::cout << "total valid pixs in gt: " << pix_count << " number of valid disp results in gen_disp: " << 
	pix_gen_count << " density of generated results: " << density << std::endl;
	/***** End of Disp Error Outliers *****/

	//float* error = dispErrorAvg(d_gt, d_gen, dmax,masked);
	//std::cout << "Avg error of all pixs: " << error[0] << " Avg error of valid disp results: " << error[1] << std::endl;
}

float* dispErrorAvg(cv::Mat& d_gt, cv::Mat& d_gen, int dmax, bool masked){
	
	int height = d_gt.size().height;
	int width = d_gt.size().width;
	float* error = new float[2];				//0 entry: err_total_px ,  1 entry: err_valid_px
	for(int i=0;i<2;i++) error[i]=0;
	int pix_count = 0;	//Total number of valid pixels in ground truth
	int pix_gen_count = 0;	//Total number of valid pixels in generated disparity

	if(height != d_gen.size().height || width != d_gen.size().width){
		std::cerr << "disparity image sizes don't match!" << std::endl;
		throw 1;
	}

	/***** Disp Error Outliers *****/
	for(int j=0; j<height;j++){
		for(int i=dmax; i<width; i++){
			if(masked){
				if(mask.at<uchar>(j,i)==0) continue;
			}
			float fd_gt = d_gt.at<float>(j,i);
			float fd_gen = d_gen.at<float>(j,i);
			if(isValid(fd_gt)) {
				pix_count++;
				float err = fabs(fd_gt - (fd_gen < 0 ? 0 : fd_gen));
				error[0] += err;
				if(mydValid(fd_gen)){
					pix_gen_count++;
					error[1] += err; 
				}
			}	
		}
	}

	//normalize errors
	error[0] /= std::max((float)pix_count,1.0f);
	error[1] /= std::max((float)pix_gen_count,1.0f);
	return error;
}


