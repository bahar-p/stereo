#include <stdlib.h>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <fstream>

//Global Variables
cv::Mat mask;
std::fstream of1,of2,of3;

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
		std::cerr << "Usage: ./eval gt_disp gen_disp maxDisp focal baseline ?Adcensusflag? ?is_noc? ?mask?" << std::endl;
		return -1;
	}
	cv::Mat im_gt = cv::imread(argv[1],-1);
	cv::Mat im_gen = cv::imread(argv[2],-1);
	char* fullpath = argv[1];
	char* bname = basename(fullpath);
	char* x = strtok(bname, ".");
	const std::string fname(reinterpret_cast<char*>(x));
	
	int dmax = atoi(argv[3]);
	float f = atof(argv[4]);
	float bs = atof(argv[5]);
	std::cout << "F: " << f << " BS: " << bs << std::endl;
	if(argc>6) adcensus=atoi(argv[6]);
	bool masked = false;
	int noc=1;
	if(argc>7) {
		noc = atoi(argv[7]);
		if(argc==9){
			mask = cv::imread(argv[8],0);
			masked = true;
		}
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
	int pix_gen_count = 0;				//Total number of valid pixels in generated disparity
	float stAcuity_1729=32;				//Avg Stereo Acuity 17-29 age = 32arcsec
	float stAcuity_3049=33.75;			//Avg Stereo Acuity 17-29 age = 33.75arcsec
	float stAcuity_5069=38.75;			//Avg Stereo Acuity 17-29 age = 38.75arcsec
	float stAcuity_7083=112.5;			//Avg Stereo Acuity 17-29 age = 112.5arcsec
	float pupil_dist = 64; 				//Avg value of Interpupillary distance (mm)
	float c = 3437.75; 				//constant
	float err_thr[4]={stAcuity_1729, stAcuity_3049,stAcuity_5069, stAcuity_7083};		//Different error thresholds
	
	//std::cout << "d_gen: " << d_gen(cv::Rect(500,150, 80,1)) << std::endl;
	//std::cout << "d_gt: " << d_gt(cv::Rect(500,150, 80,1)) << std::endl;
	if(height != d_gen.size().height || width != d_gen.size().width){
		std::cerr << "disparity image sizes don't match!" << std::endl;
		return -1;
	}
	std::string res1,res2,res3; 
	if(!adcensus){
		if(noc){
			res1 =  "/home/bahar/Master/stereo/eval/results/noc/sgbm_rdep" + fname + ".txt";
			res2 =  "/home/bahar/Master/stereo/eval/results/noc/sgbm_outliers.txt";
			res3 =  "/home/bahar/Master/stereo/eval/results/noc/sgbm_avgerr.txt";
		} else {
			res1 =  "/home/bahar/Master/stereo/eval/results/occ/sgbm_rdep" + fname + ".txt";
			res2 =  "/home/bahar/Master/stereo/eval/results/occ/sgbm_outliers.txt";
			res3 =  "/home/bahar/Master/stereo/eval/results/occ/sgbm_avgerr.txt";
		}
	} else {
		if(noc){
			res1 =  "/home/bahar/Master/stereo/eval/results/noc/adcen_rdep" + fname + ".txt";
			res2 =  "/home/bahar/Master/stereo/eval/results/noc/adcen_outliers.txt";
			res3 =  "/home/bahar/Master/stereo/eval/results/noc/adcen_avgerr.txt";
		} else {
			res1 =  "/home/bahar/Master/stereo/eval/results/occ/adcen_rdep" + fname + ".txt";
			res2 =  "/home/bahar/Master/stereo/eval/results/occ/adcen_outliers.txt";
			res3 =  "/home/bahar/Master/stereo/eval/results/occ/adcen_avgerr.txt";
		}
	}
	of1.open(res1, std::ios::out);
	of2.open(res2, std::ios::out | std::ios::app);
	of3.open(res3, std::ios::out | std::ios::app);
	
	float INF = std::numeric_limits<float>::infinity();
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
				float dz_5069 = ((depth*depth)*(stAcuity_5069/60))/(c*pupil_dist);
				float dz_7083 = ((depth*depth)*(stAcuity_7083/60))/(c*pupil_dist);
				float depth_gen;	
				if(fd_gen!=0)
					depth_gen = (f*bs)/fd_gen;
				else depth_gen = INF;
				float depth_err;
				//if(depth_gen == INF) depth_err =0;
				depth_err = fabs(depth-depth_gen);
				float err_stAc = c*pupil_dist*(depth_err/(depth*depth));
				//std::cout << "disp_err: " << err << "  depth: " << depth_err << "  dz_1729: "  << dz_1729 << " dz_3049: " << dz_3049 << std::endl;
				//Write to output file
				/*of1 << (depth/100) << "  "  << (dz_1729/100) << "  " << (dz_3049/100) << "  " << (dz_5069/100) << "  "  << (dz_7083/100) << "  " 
					<< (depth_err==INF ? 1000000 : (depth_err/100)) <<  std::endl;*/
				if(depth_err != INF){
					of1 << (depth/100) << "  "  << (stAcuity_1729/60) << "  " << (stAcuity_3049/60) << "  " << (stAcuity_5069/60) << "  "  << (stAcuity_7083/60) << "  " 
					<< err_stAc <<  std::endl;
				}
				
				
				if(depth_err >= dz_1729){
					err_total_pxs[0]++;
				}
				if(depth_err >= dz_3049){
					err_total_pxs[1]++;
				}
				if(depth_err >= dz_5069){
					err_total_pxs[2]++;
				}
				if(depth_err >= dz_7083){
					err_total_pxs[3]++;
				}
				else {
					//std::cout << "depth out of defined ranges" << std::endl;
				}
				if(mydValid(fd_gen)){
					pix_gen_count++;
					if(depth_err >= dz_1729){
						err_valid_pxs[0]++;
					}
					if(depth_err >= dz_3049){
						err_valid_pxs[1]++;
					}
					if(depth_err >= dz_5069){
						err_valid_pxs[2]++;
					}
					if(depth_err >= dz_7083){
						err_valid_pxs[3]++;
					}
					else {
						//std::cout << "depth out of defined ranges" << std::endl;
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
		std::cout <<  "outliers for all and just valid disp results for stAcuity " <<  err_thr[i]  << " are: " << 
		err_total_pxs[i] << "  " <<  err_valid_pxs[i] << std::endl;
		of2 << "img  "  << fname+".png" << "  stAcuity  " << err_thr[i] << "  total_outl  " << err_total_pxs[i]  << "  valid_outl  " <<  err_valid_pxs[i]<< std::endl;
	}
	density = (float)pix_gen_count/std::max((float)pix_count,1.0f);
	std::cout << "total valid pixs in gt: " << pix_count << " number of valid disp results in gen_disp: " << 
	pix_gen_count << " density of generated results: " << density << std::endl;
	/***** End of Disp Error Outliers *****/

	float* error = dispErrorAvg(d_gt, d_gen, dmax,masked);
	std::cout << "Avg error of all pixs: " << error[0] << " Avg error of valid disp results: " << error[1] << std::endl;
	of3 <<"img  " << fname+".png" << "  total_err  "  << error[0] << "  valid_err  " << error[1] << std::endl;
	of1.close();
	of2.close();
	of3.close();
	//system(("tclsh ../eval/myplot.tcl " + fname).c_str());
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


