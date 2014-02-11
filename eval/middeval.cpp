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
		std::cerr << "Usage: ./eval gt_disp gen_ADdisp gen_SGdisp maxDisp" << std::endl;
		return -1;
	}
	cv::Mat im_gt = cv::imread(argv[1],-1);
	cv::Mat im_adcgen = cv::imread(argv[2],-1);
	cv::Mat im_sgmgen = cv::imread(argv[3],-1);
	int dmax = atoi(argv[4]);	
	if(!im_gt.data || !im_adcgen.data || !im_sgmgen.data){
		std::cerr << "Invalid Image" << std::endl;
		return -1;
	}
	
	cv::Mat d_gt, d_adcgen, d_sgmgen, im_diff1,im_diff2,im_diff3;
	im_gt.convertTo(d_gt, CV_32F,1/(4.));
	//d_gt = im_gt;
	im_adcgen.convertTo(d_adcgen, CV_32F,dmax/(16.*255)); 		//if it's AdCensus generated disparity
	im_sgmgen.convertTo(d_sgmgen, CV_32F,dmax/(255.)); 			//if it's sgbm generate disparity
	im_diff1 = cv::Mat(d_gt.size(), CV_32F, cv::Scalar::all(0));
	im_diff2 = cv::Mat(d_gt.size(), CV_32F, cv::Scalar::all(0));
	im_diff3 = cv::Mat(d_gt.size(), CV_32F, cv::Scalar::all(0));
	int height = d_gt.size().height;
	int width = d_gt.size().width;
	
	float err_thr[6]={0.5,0.75,1.0,1.25,1.5,1.75};		//Different error thresholds
	int arr_len = 6;
	//int arr_len = 10;
	float err_total_pxs[2*arr_len];			//Number of incorrect pixels corresponding to each threshold
	float err_valid_pxs[2*arr_len];			//Number of incorrect valid pixels in the result corresponding to each threshold
	float density =0;				//density of disparity results
	for(int i=0;i<2*arr_len ;i++){
		err_total_pxs[i]=0;
		err_valid_pxs[i]=0;
	}
	int pix_count = 0;				//Total number of valid pixels in ground truth
	int pix_gen_count = 0;		
	
	std::cout << "sampleGT: " << d_gt(cv::Rect(100,100,10,5)) << std::endl;
	std::cout << "generated_adcdisp: " << d_adcgen(cv::Rect(100,100,10,5)) << std::endl;
	std::cout << "generated_sgmdisp: " << d_sgmgen(cv::Rect(100,100,10,5)) << std::endl;
	float INF = std::numeric_limits<float>::infinity();
	double min1, max1, min2, max2, min3, max3;
	cv::minMaxLoc(d_gt,&min1,&max1);
	cv::minMaxLoc(d_adcgen,&min2,&max2);
	cv::minMaxLoc(d_sgmgen,&min3,&max3);
	std::cout<< "gt_min and max: " << min1 <<  "  " << max1 << "  " << " adc_gen_min and max: " << min2 << "  " << max2 << 
		" sgm_gen_min and max: " << min3 << "  " <<  max3 << std::endl;
	/***** Disp Error Outliers *****/
	for(int j=0; j<height;j++){
		for(int i=dmax; i<width; i++){
			float fd_gt = d_gt.at<float>(j,i);
			float fd_adcgen = d_adcgen.at<float>(j,i);
			float fd_sgmgen = d_sgmgen.at<float>(j,i);
			if(isValid(fd_gt)) {
				pix_count++;
				if(fd_sgmgen < 0 || fd_adcgen < 0) std::cout << "Negative value!" << std::endl;
				float err1 = fabs(fd_gt - (fd_adcgen < 0 ? 0 : fd_adcgen));
				float err2 = fabs(fd_gt - (fd_sgmgen < 0 ? 0 : fd_sgmgen));
				float err3 = fabs( (fd_sgmgen <0 ? 0 : fd_sgmgen) - (fd_adcgen<0 ? 0 : fd_adcgen));
				im_diff3.at<float>(j,i) = err3;
				im_diff2.at<float>(j,i) = err2;
				im_diff1.at<float>(j,i) = err1;
				for(int k=0;k<arr_len;k++){
					if(err1 > err_thr[k])
						err_total_pxs[k]++;
					if(err2 > err_thr[k])
						err_total_pxs[k+arr_len]++;
				}
				if(mydValid(fd_adcgen) && mydValid(fd_sgmgen)){
					pix_gen_count++;
					for(int k=0;k<arr_len;k++){
						if(err1 > err_thr[k])
							err_valid_pxs[k]++;
						if(err2 > err_thr[k])
							err_valid_pxs[k+arr_len]++;
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
	for(int i=0;i<2*arr_len;i++){
		err_total_pxs[i] /= std::max((float)pix_count, 1.0f);
		if(pix_gen_count > 0)
			err_valid_pxs[i] /= std::max((float)pix_gen_count,1.0f);
	}
	for(int i=0;i<arr_len;i++){
		std::cout <<  "outliers for all and just valid disp results for threshold " <<  err_thr[i]  << " are: " << 
		" adcensus: "   << err_total_pxs[i] <<  "  " <<  err_valid_pxs[i] << " sgbm: "<<  err_total_pxs[i+arr_len] 
		<<  "  " <<  err_valid_pxs[i+arr_len] << std::endl;
	}
	density = (float)pix_gen_count/std::max((float)pix_count,1.0f);
	std::cout << "total valid pixs in gt: " << pix_count << " number of valid disp results in gen_disp: " << 
	pix_gen_count << " density of generated results: " << density <<std::endl;

	cv::Mat im3_diff8, im2_diff8, im1_diff8;
	double min4,max4;
	cv::minMaxLoc(im_diff3,&min4,&max4);
	im_diff3.convertTo(im3_diff8, CV_8U,255./max4);
	cv::imwrite("/home/bahar/im_diff.png", im3_diff8);

	cv::minMaxLoc(im_diff2,&min4,&max4);
	im_diff2.convertTo(im2_diff8, CV_8U,255./max4);
	cv::imwrite("/home/bahar/sgm_diff.png", im2_diff8);

	cv::minMaxLoc(im_diff1,&min4,&max4);
	im_diff1.convertTo(im1_diff8, CV_8U,255./max4);
	cv::imwrite("/home/bahar/adc_diff.png", im1_diff8);

	cv::imshow("diff img" , im3_diff8);
	cv::imshow("adc img" , im1_diff8);
	cv::imshow("sgm img" , im2_diff8);
	cv::waitKey(0);
	return 0;
}
