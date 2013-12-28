#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"

float* dispErrorAvg(cv::Mat& d_gt, cv::Mat& d_gen);
bool isValid(float d){
	bool valid = (d > 0 ? true : false);
	return valid;
}

int main (int argc, char** argv) {

	if(argc!=4) {
		std::cerr << "Usage: ./eval gt_disp gen_disp maxDisp" << std::endl;
		return -1;
	}
	cv::Mat im_gt = cv::imread(argv[1],-1);
	cv::Mat im_gen = cv::imread(argv[2],-1);
	int dmax = atoi(argv[3]);
	if(!im_gt.data || !im_gen.data){
		std::cerr << "Invalid Image" << std::endl;
		return -1;
	}

	//Get the original disp values
	cv::Mat d_gt, d_gen;
	im_gt.convertTo(d_gt, CV_32F, 1/256.);
	im_gen.convertTo(d_gen, CV_32F, (dmax)/255.);

	int height = d_gt.size().height;
	int width = d_gt.size().width;
	float err_thr[4]={0.5,1,2,3};		//Different error thresholds
	int arr_len = sizeof(err_thr)/sizeof(*err_thr);
	float err_total_pxs[arr_len];			//Number of incorrect pixels corresponding to each threshold
	float err_valid_pxs[arr_len];			//Number of incorrect pixels corresponding to each threshold
	float density =0;				//density of disparity results
	for(int i=0;i<arr_len ;i++){
		err_total_pxs[i]=0;
		err_valid_pxs[i]=0;
	}
	int pix_count = 0;	//Total number of valid pixels in ground truth
	int pix_gen_count = 0;	//Total number of valid pixels in generated disparity

	//std::cout << "d_gen: " << d_gen(cv::Rect(400,200, 10,10)) << std::endl;
	//std::cout << "d_gt: " << d_gt(cv::Rect(400,200, 10,10)) << std::endl;
	if(height != d_gen.size().height || width != d_gen.size().width){
		std::cerr << "disparity image sizes don't match!" << std::endl;
		return -1;
	}
	
	/***** Disp Error Outliers *****/
	for(int j=0; j<height;j++){
		for(int i=0; i<width; i++){
			float fd_gt = d_gt.at<float>(j,i);
			float fd_gen = d_gen.at<float>(j,i);
			if(isValid(fd_gt)) {
				pix_count++;
				float err = fabs(fd_gt - fd_gen);
				//std::cout << "err: " << err << std::endl;
				for(int k=0; k<arr_len; k++){	
					if(err > err_thr[k]) err_total_pxs[k]++; 
				}
				if(isValid(fd_gen)){
					pix_gen_count++;
					for(int k=0; k<arr_len; k++){		
						if(err > err_thr[k]) err_valid_pxs[k]++; 
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
		std::cout <<  "outliers for all and just valid disp results= " << err_thr[i] << " are: " << err_total_pxs[i] << "  " <<  err_valid_pxs[i] << std::endl;
	}
	density = (float)pix_gen_count/std::max((float)pix_count,1.0f);
	std::cout << "total valid pixs in gt: " << pix_count << " number of valid disp results in gen_disp: " << pix_gen_count << " density of generated results: " << density << std::endl;
	/***** End of Disp Error Outliers *****/

	float* error = dispErrorAvg(d_gt, d_gen);
	std::cout << "Avg error of all pixs: " << error[0] << " Avg error of valid disp results: " << error[1] << std::endl;
}

float* dispErrorAvg(cv::Mat& d_gt, cv::Mat& d_gen){
	
	int height = d_gt.size().height;
	int width = d_gt.size().width;
	float* error = new float[2];				//0 entry: err_total_px ,  1 entry: err_valid_px
	for(int i=0;i<2;i++) error[i]=0;
	int pix_count = 0;	//Total number of valid pixels in ground truth
	int pix_gen_count = 0;	//Total number of valid pixels in generated disparity

	//std::cout << "d_gen: " << d_gen(cv::Rect(400,200, 10,10)) << std::endl;
	//std::cout << "d_gt: " << d_gt(cv::Rect(400,200, 10,10)) << std::endl;
	if(height != d_gen.size().height || width != d_gen.size().width){
		std::cerr << "disparity image sizes don't match!" << std::endl;
		throw 1;
	}
	
	/***** Disp Error Outliers *****/
	for(int j=0; j<height;j++){
		for(int i=0; i<width; i++){
			float fd_gt = d_gt.at<float>(j,i);
			float fd_gen = d_gen.at<float>(j,i);
			if(isValid(fd_gt)) {
				pix_count++;
				float err = fabs(fd_gt - fd_gen);
				error[0] += err;
				if(isValid(fd_gen)){
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


