#include "image.h"

using namespace std;
using namespace cv;


image::image(Mat image_leftRGB, Mat image_rightRGB, int dMin, int dMax){
	img_leftRGB=image_leftRGB;
	img_rightRGB=image_rightRGB;
	dispMin = dMin;
	dispMax = dMax;
	Size s = img_leftRGB.size();
	
	int sz[] = {s.height, s.width, dispMax-dispMin+1};
	//std::cout << "Maximum value for double: " << std::numeric_limits<double>::max() << '\n';
	//std::cout << "Maximum value for long double: " << std::numeric_limits<long double>::max() << '\n';
	//std::cout << "height: " << s.height << " , " <<  img_leftRGB.rows<< '\n';
	//std::cout << "width: " << s.width << " , " <<  img_leftRGB.cols<< '\n';

	
	//Initialization of different cost space
	
	//color_left=cv::Mat(s, CV_8UC3, cv::Scalar::all(0));
	//color_right=cv::Mat(s, CV_8UC3,cv::Scalar::all(0));
	DSI=cv::Mat(3, sz, mytype,cv::Scalar::all(0));
	init_cost=cv::Mat(3, sz, mytype,cv::Scalar::all(0));
	aggr_cost=cv::Mat(3, sz, mytype, cv::Scalar::all(0));
	final_cost=cv::Mat(3, sz, mytype, cv::Scalar::all(0));
	left_cost=cv::Mat(3, sz, mytype, cv::Scalar::all(0));
	right_cost=cv::Mat(3, sz, mytype, cv::Scalar::all(0));
	up_cost=cv::Mat(3, sz, mytype, cv::Scalar::all(0));
	down_cost=cv::Mat(3, sz, mytype, cv::Scalar::all(0));
	HII=cv::Mat(3, sz , mytype, cv::Scalar::all(0));
	VII=cv::Mat(3, sz, mytype, cv::Scalar::all(0));
	sumH=cv::Mat(3, sz, mytype, cv::Scalar::all(0));
	sumV=cv::Mat(3, sz, mytype, cv::Scalar::all(0));
	census_hamming = (unsigned***)malloc(img_leftRGB.rows*sizeof(unsigned**));
	censusLeft = (uint64_t**) malloc(img_leftRGB.rows*sizeof(uint64_t*));
	censusRight = (uint64_t**) malloc(img_leftRGB.rows*sizeof(uint64_t*));
	
	int d[]={s.height, s.width, 4};
	supReg=cv::Mat(3, d , CV_32SC1, cv::Scalar::all(0));
	
	for (int i = 0 ;  i < img_leftRGB.rows; i++) {
		censusLeft[i] = (uint64_t*) malloc(sizeof(uint64_t) * img_leftRGB.cols);			//Left image Census Cost
		censusRight[i] = (uint64_t*) malloc(sizeof(uint64_t) * img_leftRGB.cols);			//Right image Census Cost
		census_hamming[i] = (unsigned**) malloc(sizeof(unsigned*) * img_leftRGB.cols);		//Hamming distance for census cost
		for (int j = 0; j < img_leftRGB.cols; j++){
			census_hamming[i][j]= (unsigned *)malloc(sizeof(unsigned) * (dispMax-dispMin+1));
		}
	}
	for(int p= 0 ; p<img_leftRGB.rows ; p++){
		for(int q= 0 ; q<img_leftRGB.cols ; q++){
			censusLeft[p][q]=0;
			censusRight[p][q]=0;
			for(int d=0;d<dispMax-dispMin+1;d++){		
				census_hamming[p][q][d]=0;
			}
		}
	}
	
	
	//DEBUG
	/*init_cost.at<double>(1,1)=1;
	init_cost.at<double>(1,2)=3;
	init_cost.at<double>(1,3)=2;
	init_cost.at<double>(1,4)=0;
	init_cost.at<double>(2,1)=5;
	init_cost.at<double>(2,2)=8;
	init_cost.at<double>(2,3)=1;
	init_cost.at<double>(2,4)=2;
	init_cost.at<double>(3,1)=1;
	init_cost.at<double>(3,2)=7;
	init_cost.at<double>(3,3)=6;
	init_cost.at<double>(3,4)=1;
	init_cost.at<double>(4,1)=0;
	init_cost.at<double>(4,2)=2;
	init_cost.at<double>(4,3)=0;
	init_cost.at<double>(4,4)=5;
	
	supReg.at<int>(1,1,0)=0;
	supReg.at<int>(1,1,1)=1;
	supReg.at<int>(1,1,2)=0;
	supReg.at<int>(1,1,3)=2;
	supReg.at<int>(1,2,0)=1;
	supReg.at<int>(1,2,1)=2;
	supReg.at<int>(1,2,2)=0;
	supReg.at<int>(1,2,3)=2;
	supReg.at<int>(1,3,0)=1;
	supReg.at<int>(1,3,1)=1;
	supReg.at<int>(1,3,2)=0;
	supReg.at<int>(1,3,3)=3;
	supReg.at<int>(1,4,0)=3;
	supReg.at<int>(1,4,1)=0;
	supReg.at<int>(1,4,2)=0;
	supReg.at<int>(1,4,3)=2;
	
	supReg.at<int>(2,1,0)=0;
	supReg.at<int>(2,1,1)=2;
	supReg.at<int>(2,1,2)=1;
	supReg.at<int>(2,1,3)=2;
	supReg.at<int>(2,2,0)=1;
	supReg.at<int>(2,2,1)=1;
	supReg.at<int>(2,2,2)=1;
	supReg.at<int>(2,2,3)=1;
	supReg.at<int>(2,3,0)=1;
	supReg.at<int>(2,3,1)=1;
	supReg.at<int>(2,3,2)=1;
	supReg.at<int>(2,3,3)=1;
	supReg.at<int>(2,4,0)=3;
	supReg.at<int>(2,4,1)=0;
	supReg.at<int>(2,4,2)=1;
	supReg.at<int>(2,4,3)=1;
	
	supReg.at<int>(3,1,0)=0;
	supReg.at<int>(3,1,1)=2;
	supReg.at<int>(3,1,2)=2;
	supReg.at<int>(3,1,3)=1;
	supReg.at<int>(3,2,0)=1;
	supReg.at<int>(3,2,1)=2;
	supReg.at<int>(3,2,2)=2;
	supReg.at<int>(3,2,3)=1;
	supReg.at<int>(3,3,0)=2;
	supReg.at<int>(3,3,1)=1;
	supReg.at<int>(3,3,2)=1;
	supReg.at<int>(3,3,3)=1;
	supReg.at<int>(3,4,0)=3;
	supReg.at<int>(3,4,1)=0;
	supReg.at<int>(3,4,2)=2;
	supReg.at<int>(3,4,3)=1;
	
	supReg.at<int>(4,1,0)=0;
	supReg.at<int>(4,1,1)=2;
	supReg.at<int>(4,1,2)=1;
	supReg.at<int>(4,1,3)=0;
	supReg.at<int>(4,2,0)=1;
	supReg.at<int>(4,2,1)=1;
	supReg.at<int>(4,2,2)=3;
	supReg.at<int>(4,2,3)=0;
	supReg.at<int>(4,3,0)=1;
	supReg.at<int>(4,3,1)=1;
	supReg.at<int>(4,3,2)=3;
	supReg.at<int>(4,3,3)=0;
	supReg.at<int>(4,4,0)=3;
	supReg.at<int>(4,4,1)=0;
	supReg.at<int>(4,4,2)=3;
	supReg.at<int>(4,4,3)=0;*/
	
}

void image::reset(){
	DSI=cv::Scalar::all(0);
	init_cost=cv::Scalar::all(0);
	aggr_cost=cv::Scalar::all(0);
	final_cost=cv::Scalar::all(0);
	left_cost=cv::Scalar::all(0);
	right_cost=cv::Scalar::all(0);
	up_cost=cv::Scalar::all(0);
	down_cost=cv::Scalar::all(0);
	HII= cv::Scalar::all(0);
	VII=cv::Scalar::all(0);
	sumH= cv::Scalar::all(0);
	sumV=cv::Scalar::all(0);
	supReg=cv::Scalar::all(0);
	for(int p= 0 ; p<img_leftRGB.rows ; p++){
		for(int q= 0 ; q<img_leftRGB.cols ; q++){
			censusLeft[p][q]=0;
			censusRight[p][q]=0;
			for(int d=0;d<dispMax-dispMin+1;d++){		
				census_hamming[p][q][d]=0;
			}
		}
	}
	
}
Mat image::get_image(int left){
	if(left==1)
		return img_leftRGB;
	else return img_rightRGB;
}

/* Calculating the average intesity difference for each pixel and its correspondence */
void image::costAD(bool dispR){
	int d,p,q;
	
	for(d=0;d<dispMax-dispMin+1;d++){
		for(p=subRH;p<img_leftRGB.rows-subRH;p++){					//Rows = height
			for(q=subRW;q<img_leftRGB.cols-subRW;q++){				//cols = width
			
			//Left disparity
				if(!dispR){
					if(q-d-dispMin>subRW-1){
						DSI.at<double>(p,q,d)= (double)((abs(img_leftRGB.at<cv::Vec3b>(p,q).val[0] - img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[0])) + 
						(abs(img_leftRGB.at<cv::Vec3b>(p,q).val[1] - img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[1])) +
						(abs(img_leftRGB.at<cv::Vec3b>(p,q).val[2] - img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[2])))/3;

						/*printf ("b: %f\t, g: %u\t, r: %u\t, bR: %u\t, gR: %u\t , rR: %u\t" , (double)img_leftRGB.at<cv::Vec3b>(p,q).val[0], img_leftRGB.at<cv::Vec3b>(p,q).val[1],
						img_leftRGB.at<cv::Vec3b>(p,q).val[2], img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[0],
						img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[1],img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[2]);*/
					}
				}
				else{
					//Right disparity
					if(q+d+dispMin<img_leftRGB.cols-subRW){
						DSI.at<double>(p,q,d)= (double)((abs(img_leftRGB.at<cv::Vec3b>(p,q+d+dispMin).val[0] - img_rightRGB.at<cv::Vec3b>(p,q).val[0])) + 
						(abs(img_leftRGB.at<cv::Vec3b>(p,q+d+dispMin).val[1] - img_rightRGB.at<cv::Vec3b>(p,q).val[1])) +
						(abs(img_leftRGB.at<cv::Vec3b>(p,q+d+dispMin).val[2] - img_rightRGB.at<cv::Vec3b>(p,q).val[2])))/3;
					}
				}
			}
		}
	}
}

/* Encoding each pixel local structure */
void image::costCensus(int winX, int winY, int left){
	uint64_t census;
	uint64_t bit=0;
	int shifts;
	cv::Mat left_gray, right_gray;
	cvtColor(img_leftRGB,left_gray,CV_BGR2GRAY);
	cvtColor(img_rightRGB,right_gray,CV_BGR2GRAY);
	for(int x = winX/2; x<img_leftRGB.rows - winX/2; x++){
		for(int y = winY/2; y<img_leftRGB.cols - winY/2 ; y++){
			//printf("x: %d\t , y: %d\t" , x,y);
			census=0;
			shifts=0;
			for(int i=x - winX/2; i <= x + winX/2 ; i++){
				for(int j = y - winY/2; j <= y + winY/2 ; j++){
					if(shifts != winX*winY/2){						//just to exclude the central pixel from the calculation
						census <<= 1;	
						if(left==1){								//left image		
							if(left_gray.at<uchar>(i,j)<left_gray.at<uchar>(x,y))
								bit=1;
							else bit=0;
						} else {
							if(right_gray.at<uchar>(i,j)<right_gray.at<uchar>(x,y))
								bit=1;
							else bit=0;
						}
						census = census+bit;
						//printf("census: %lld\n" , census);
					}
					shifts++;
				}
			}
			if(left==1)
				censusLeft[x][y]= census;
			else censusRight[x][y]= census;
			/*if(x==23 && y==60)
				printf("x: %d\t , y: %d\t , censusL: %s\n" , x,y, itob(censusLeft[x][y]));
			if(x==23 && y==58)
				printf("x: %d\t , y: %d\t , censusR: %s\n" , x,y, itob(censusRight[x][y]));*/
		}
	}
}

char * image::itob(uint64_t x)
{
	cout << sizeof(uint64_t) << endl;
	static char buff[sizeof(uint64_t) * CHAR_BIT + 1];
	int i;
	for(i=0;i<sizeof(uint64_t) * CHAR_BIT; i++)
	{
		uint64_t unit = 1;
		if(x & (unit << (sizeof(uint64_t) * CHAR_BIT -1- i)))
			buff[i] = '1';
		else
			buff[i] = '0';
	}
	cout << "length: " << i << endl;
	return buff;
}

/* Calculating the hamming distance between each pixel and its correspondence census cost */
void image::hamdist(uint64_t** censL, uint64_t** censR, int winX, int winY, bool dispR){
	int d,p,q;
	uint64_t val=0;
	unsigned dist=0;
	for(d=0;d<dispMax-dispMin+1;d++){
		for(p= winX/2 ; p<img_leftRGB.rows - winX/2 ; p++){					//Rows = height
			for(q= winY/2 ; q<img_leftRGB.cols - winY/2 ; q++){				//cols = width
				dist=0;
				val=0;
				if(!dispR){
					if(q-d-dispMin>subRW -1){
						val = censL[p][q] ^ censR[p][q-d-dispMin];				//XOR operation
						//printf("censL: %lld\t\t , censR: %lld\t\t", censL[p][q+d+dispMin], censR[p][q]);
						while(val){
							++dist;
							val &= val - 1;
						}
						census_hamming[p][q][d]= dist;
						//printf("hammingDist: %u\t\t", dist);
						/*if(p==23 && q==60 && (q-d-dispMin==58)){
							printf("x: %d\t , q: %d\t , census_hamming: %u\n" , p,q-d-dispMin, census_hamming[p][q][d]);
						}*/
					}
				}
				else{
					if(q+d+dispMin<img_leftRGB.cols-subRW){
						val = censL[p][q+d+dispMin] ^ censR[p][q];				//XOR operation
						//printf("censL: %lld\t\t , censR: %lld\t\t", censL[p][q+d+dispMin], censR[p][q]);
						while(val){
							++dist;
							val &= val - 1;
						}
						census_hamming[p][q][d]= dist;
						//printf("hammingDist: %u\t\t", dist);
					}
				}
			}
		}
		
	}
	
	//std::cout<< "census_hamming(100,200,10): " << census_hamming[100][200][10] << std::endl;
}

/* Calculate census cost */
void image::c_census(int X, int Y, bool dispR){
	
	hamdist(censusLeft, censusRight, X,Y, dispR);
	
}
/* Calculating the initial cost: Census + AD */
void image::initCost(double lam_AD, double lam_census){
	std::cout<< "AdCensus algorithm..." <<std::endl;
	int p,q,d;
	for(d=0;d<dispMax-dispMin+1;d++){
		for(p= subRH ; p<img_leftRGB.rows-subRH ; p++){					//Rows = height
			for(q= subRW ; q<img_leftRGB.cols-subRW ; q++){				//cols = width
				init_cost.at<double>(p,q,d)= 2.0 - exp(-DSI.at<double>(p,q,d)/lam_AD) - exp(-(double)census_hamming[p][q][d]/lam_census);
				if(init_cost.at<double>(p,q,d) > 2.0)
					abort();
			}
		}
		
	}
	//std::cout<< "init_cost(100,200,10): " << init_cost.at<double>(100,200,10) << std::endl;
	//printf("DSI: %f\t\t , census: %f\t\t , cost: %f\t\t\n ", DSI[0][0][0], (double)census_hamming[0][0][0],init_cost[0][0][0] );
}

/* Building the cross-based region for each pixel */
void image::line_segment(double colLim1, double colLim2, double distLim1, double distLim2, bool dispR){
	int p,q,x,y;
	bool arm_found=false;
	for(p= subRH ; p<img_leftRGB.rows-subRH; p++){					//Rows = height
		for(q= subRW ; q<img_leftRGB.cols-subRW ; q++){				//cols = width
			
			if(!dispR){
				arm_found=false;
				if(q!=subRW){
					int dist;
					for(y=q-1; y>=subRW; y--){							//scan left arm - The first arg
						double col_diff1 = colDiffer(img_leftRGB, p,y,p,q);
						double col_diff2 = colDiffer(img_leftRGB, p,y,p,y+1);		//the endpoint and its predecessor
						dist= std::abs(y-q);					
						
						if(!(col_diff1<colLim1 && col_diff2<colLim1) || !(dist<distLim1) || (dist<distLim1 && dist>distLim2 && !(col_diff1<colLim2))){	//if violates any of these rules
							 supReg.at<int>(p,q,0)=dist-1;
							 arm_found=true;
							 //std::cout<< "For the point (" << p << " , "  << q << ") The left arm endpoint is: " << dist << std::endl;
							 break;
						}
					}
					if( supReg.at<int>(p,q,0)==0 && !arm_found){							//if no endpoint found and it's not one of the edges' pixels
						//int dist = std::abs(subRW-q);
						supReg.at<int>(p,q,0)= dist;										//The edge pixel will be the end point.
						//std::cout<< "excep: For the point (" << p << " , "  << q << ") The left arm endpoint is: " << dist << std::endl;
					}
				}
				arm_found=false;
				if(q!=img_leftRGB.cols-subRW-1){
					int dist;
					for(y=q+1; y<img_leftRGB.cols - subRW; y++){							//scan right arm - The second arg
						double col_diff1 = colDiffer(img_leftRGB, p,y,p,q);
						double col_diff2 = colDiffer(img_leftRGB,p,y,p,y-1);						//the end point and its predecessor
						dist= std::abs(y-q);
						
						if(!(col_diff1<colLim1 && col_diff2<colLim1) || !(dist<distLim1) || (dist<distLim1 && dist>distLim2 && !(col_diff1<colLim2))){
							 supReg.at<int>(p,q,1)=dist-1;
							 arm_found=true;
							// std::cout<< "For the point (" << p << " , "  << q << ") The right arm endpoint is: " << dist << std::endl;
							 break;
						}
					}
				
					if( supReg.at<int>(p,q,1)==0 && !arm_found){	
						//int dist = std::abs(img_leftRGB.cols-1-subRW-q);	
						supReg.at<int>(p,q,1)=dist;										
						//std::cout<< "excep: For the point (" << p << " , "  << q << ") The right arm endpoint is: " << dist << std::endl;
					}
				}
				arm_found=false;
				if(p!=subRH){
					int dist;
					for(x=p-1; x>=subRH; x--){											//scan up arm - The third arg
						double col_diff1 = colDiffer(img_leftRGB,x,q,p,q);
						double col_diff2 = colDiffer(img_leftRGB,x,q,x+1,q);
						dist= std::abs(x-p);
						
						if(!(col_diff1<colLim1 && col_diff2<colLim1) || !(dist<distLim1) || (dist<distLim1 && dist>distLim2 && !(col_diff1<colLim2))){
							  supReg.at<int>(p,q,2)=dist-1;
							  arm_found = true;
							 //std::cout<< "For the point (" << p << " , "  << q << ") The up arm	endpoint is: " << dist << std::endl;
							 break;
						}
					}
				
					if( supReg.at<int>(p,q,2)==0 && !arm_found){
						//int dist = std::abs(subRH-p);
						supReg.at<int>(p,q,2)= dist;										
						//std::cout<< "excep: For the point (" << p << " , "  << q << ") The up arm endpoint is: " << dist << std::endl;
					}
				}
				arm_found=false;
				if(p!=img_leftRGB.rows-subRH-1){
					int dist;
					for(x=p+1; x<img_leftRGB.rows-subRH; x++){							//scan bottom arm - The fourth arg
						double col_diff1 = colDiffer(img_leftRGB,x,q,p,q);
						double col_diff2 = colDiffer(img_leftRGB,x,q,x-1,q);						//the end point and its predecessor
						dist= std::abs(x-p);
						
						if(!(col_diff1<colLim1 && col_diff2<colLim1) || !(dist<distLim1) || (dist<distLim1 && dist>distLim2 && !(col_diff1<colLim2))){
							 supReg.at<int>(p,q,3)=dist-1;
							 arm_found=true;
							//std::cout<< "For the point (" << p << " , "  << q << ") The bottom arm endpoint is: " << dist << std::endl;
							break;
						}
					}
				if( supReg.at<int>(p,q,3)==0 && !arm_found){
					//int dist = std::abs(img_leftRGB.rows-1-subRH-p);
					supReg.at<int>(p,q,3)= dist;
					//std::cout<< "excep: For the point (" << p << " , "  << q << ") The bottom arm endpoint is: " << dist << std::endl;
				}
			}
			} else {
				arm_found=false;
				if(q!=subRW){
					int dist;
					for(y=q-1; y>=subRW; y--){							//scan left arm - The first arg
						double col_diff1 = colDiffer(img_rightRGB, p,y,p,q);
						double col_diff2 = colDiffer(img_rightRGB, p,y,p,y+1);		//the endpoint and its predecessor
						dist= std::abs(y-q);					
						if(!(col_diff1<colLim1 && col_diff2<colLim1) || !(dist<distLim1) || (dist<distLim1 && dist>distLim2 && !(col_diff1<colLim2))){	//if violates any of these rules
							 supReg.at<int>(p,q,0)=dist-1;
							 arm_found = true;
							 //std::cout<< "For the point (" << p << " , "  << q << ") The left arm endpoint is: " << dist << std::endl;
							 break;
						}
					}
					if( supReg.at<int>(p,q,0)==0 && !arm_found){							//if no endpoint found and it's not one of the edges' pixels
						 //int dist = std::abs(subRW-q);
						 supReg.at<int>(p,q,0)= dist;										//The edge pixel will be the end point.
						//std::cout<< "excep: For the point (" << p << " , "  << q << ") The left arm endpoint is: " << dist << std::endl;
					}
				}
				arm_found=false;
				if(q!=img_leftRGB.cols-subRW-1){
					int dist;
					for(y=q+1; y<img_leftRGB.cols - subRW; y++){							//scan right arm - The second arg
						double col_diff1 = colDiffer(img_rightRGB, p,y,p,q);
						double col_diff2 = colDiffer(img_rightRGB,p,y,p,y-1);						//the end point and its predecessor
						dist= std::abs(y-q);
						
						if(!(col_diff1<colLim1 && col_diff2<colLim1) || !(dist<distLim1) || (dist<distLim1 && dist>distLim2 && !(col_diff1<colLim2))){
							  supReg.at<int>(p,q,1)=dist-1;
							  arm_found=true;
							// std::cout<< "For the point (" << p << " , "  << q << ") The right arm endpoint is: " << dist << std::endl;
							 break;
						}
					}
					if( supReg.at<int>(p,q,1)==0 && !arm_found){	
						//int dist = std::abs(img_leftRGB.cols-1-subRW-q);	
						 supReg.at<int>(p,q,1)=dist;										
						//std::cout<< "excep: For the point (" << p << " , "  << q << ") The right arm endpoint is: " << dist << std::endl;
					}
				}
				arm_found=false;
				if(p!=subRH){
					int dist;
					for(x=p-1; x>=subRH; x--){											//scan up arm - The third arg
						double col_diff1 = colDiffer(img_rightRGB,x,q,p,q);
						double col_diff2 = colDiffer(img_rightRGB,x,q,x+1,q);
						dist= std::abs(x-p);
						
						if(!(col_diff1<colLim1 && col_diff2<colLim1) || !(dist<distLim1) || (dist<distLim1 && dist>distLim2 && !(col_diff1<colLim2))){
							  supReg.at<int>(p,q,2)=dist-1;
							  arm_found=true;
							 //std::cout<< "For the point (" << p << " , "  << q << ") The up arm	endpoint is: " << dist << std::endl;
							 break;
						}
					}
					if( supReg.at<int>(p,q,2)==0 && !arm_found){
						//int dist = std::abs(subRH-p);
						 supReg.at<int>(p,q,2)= dist;										
						//std::cout<< "excep: For the point (" << p << " , "  << q << ") The up arm endpoint is: " << dist << std::endl;
					}
				}
				arm_found=false;
				if(p!=img_leftRGB.rows-subRH-1){
					int dist;
					for(x=p+1; x<img_leftRGB.rows-subRH; x++){							//scan bottom arm - The fourth arg
						double col_diff1 = colDiffer(img_rightRGB,x,q,p,q);
						double col_diff2 = colDiffer(img_rightRGB,x,q,x-1,q);						//the end point and its predecessor
						dist= std::abs(x-p);
						
						if(!(col_diff1<colLim1 && col_diff2<colLim1) || !(dist<distLim1) || (dist<distLim1 && dist>distLim2 && !(col_diff1<colLim2))){
							 supReg.at<int>(p,q,3)=dist-1;
							 arm_found=true;
							//std::cout<< "For the point (" << p << " , "  << q << ") The bottom arm endpoint is: " << dist << std::endl;
							break;
						}
					}
					if( supReg.at<int>(p,q,3)==0 && !arm_found){
						//int dist = std::abs(img_leftRGB.rows-1-subRH-p);
						 supReg.at<int>(p,q,3)= dist;
						//std::cout<< "excep: For the point (" << p << " , "  << q << ") The bottom arm endpoint is: " << dist << std::endl;
					}
				}
			}
		}
	}
}

/* Calculating aggregated cost */
void image::aggregateCost(){
	int counter=1;
	int iter=4;
	
	while(counter<iter+1){
		switch(counter){
			case 1:
				std::cout<<"1"<<std::endl;
				IImage(init_cost, HII, 'H');
				finalSum(HII, sumH, 'H', counter);
				IImage(sumH, VII, 'V');
				finalSum(VII, sumV,'V', counter);
				//VII = cv::Scalar::all(0);
				//std::cout<< "sumV(100,200,10): " << sumV.at<double>(100,200,10) << std::endl;
			break;
			case 2:
				std::cout<<"2"<<std::endl;
				IImage(sumV, VII, 'V');
				finalSum(VII, sumV,'V', counter);
				//VII = cv::Scalar::all(0);
				IImage(sumV, HII, 'H');
				finalSum(HII, sumH, 'H', counter);
				//HII = cv::Scalar::all(0);
				//std::cout<< "sumH(100,200,10): " << sumH.at<double>(100,200,10) << std::endl;
			break;
			case 3:
				std::cout<<"3"<<std::endl;
				IImage(sumH, HII,'H');
				finalSum(HII, sumH, 'H',counter);
				//HII = cv::Scalar::all(0);
				IImage(sumH, VII, 'V');
				finalSum(VII,sumV,'V',counter);
				//VII = cv::Scalar::all(0);
				//std::cout<< "sumV(100,200,10): " << sumV.at<double>(100,200,10) << std::endl;
			break;
			case 4:
				std::cout<< "4"<<std::endl;
				IImage(sumV, VII, 'V');
				finalSum(VII, sumV,'V',counter);
				//VII = cv::Scalar::all(0);
				IImage(sumV, HII, 'H');
				finalSum(HII, sumH, 'H',counter);
				//HII = cv::Scalar::all(0);
				//std::cout<< "sumH(100,200,10): " << sumH.at<double>(100,200,10) << std::endl;
			break;
			default:
				std::cout<< "iteration out of range" <<std::endl;
			break;
		}
		counter++;
	}
	//aggr_cost = cv::Scalar::all(0);
	finalSum(sumH, aggr_cost, 'C',  counter-1);
	//std::cout<< "aggr_cost(100,200,10): " << aggr_cost.at<double>(100,200,10) << std::endl;
		
}
/* Calculating Integral Image */
void image::IImage(cv::Mat in, cv::Mat out, char dir){
	double max=0.0;
	switch (dir){
		case 'H':
			for(int d=0; d<dispMax-dispMin+1; d++){
				for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
					for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
						out.at<double>(p,q,d) =in.at<double>(p,q,d) + out.at<double>(p,q-1,d);
					}
				}
			}
			max = findMax(out);
			std::cout << "Imax: " << max << " dir:H" << std::endl;
		break;
		case 'V':
			for(int d=0; d<dispMax-dispMin+1; d++){
				for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
					for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){	
						out.at<double>(p,q,d)= in.at<double>(p,q,d) + out.at<double>(p-1,q,d);
						//printf("IImage: VII[%d][%d][%d]= %Lf \t\n", p, q, d, out[p][q][d]);
					}
				}
			}
			max = findMax(out);
			std::cout << "Imax: " << max << " dir: V" << std::endl;
		break;
		default:
			std::cout<< "none of the directions!"<< std::endl;
		break;
	}
}

/* Calculating final cost at each stage based on calculated integral image and the local support region for each pixel */
void image::finalSum(cv::Mat in, cv::Mat out, char dir, int count){
switch (dir){
		case 'H':
			for(int d=0; d<dispMax-dispMin+1; d++){
				for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
					for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
						int left= supReg.at<int>(p,q,0);				//left arm
						int right= supReg.at<int>(p,q,1);				//right arm
						out.at<double>(p,q,d) = in.at<double>(p,q+right,d)-in.at<double>(p,q-left-1,d);
					}
				}
			}
		break;
		case 'V':
			for(int d=0; d<dispMax-dispMin+1; d++){
				for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
					for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){		
						int up= supReg.at<int>(p,q,2);					//up arm
						int down= supReg.at<int>(p,q,3);				//down arm
						out.at<double>(p,q,d) = in.at<double>(p+down,q,d)-in.at<double>(p-up-1,q,d);
						
					}
				}
			}
		break;
		case 'C':											//copy to final aggregated result volume
			for(int d=0; d<dispMax-dispMin+1; d++){
				for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
					for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
						out.at<double>(p,q,d) = in.at<double>(p,q,d);
						
						if(q==subRW)
							left_cost.at<double>(p,q,d)=in.at<double>(p,q,d);
						if (q==img_leftRGB.cols-subRW-1 )
							right_cost.at<double>(p,q,d)=in.at<double>(p,q,d);
						if (p==subRH)
							up_cost.at<double>(p,q,d)=in.at<double>(p,q,d);
						if (p==img_leftRGB.rows-subRH-1)
							down_cost.at<double>(p,q,d)=in.at<double>(p,q,d);
						
						//if( p==243 && q==4)
							//printf("Final: aggcost[%d][%d][%d]= %Lf \t, iter: %d\t\n ", p, q, d, aggr_cost.at<double>(p,q,d), count);
					}
				}
			}
		break;
		default:
			std::cout << "none of the directions"<<std::endl;
		break;
	}
}

/* Calculate color difference */
double image::colDiffer(cv::Mat in, int x1, int y1, int x2, int y2){
	
	double color = (double) std::max(std::max(abs(in.at<cv::Vec3b>(x1,y1)[0]-in.at<cv::Vec3b>(x2,y2)[0]), abs(in.at<cv::Vec3b>(x1,y1)[1]-in.at<cv::Vec3b>(x2,y2)[1])), 
					abs(in.at<cv::Vec3b>(x1,y1)[2]-in.at<cv::Vec3b>(x2,y2)[2]));
					
	return color;
}

/* Scanline optimization from 4 direction: LRUD */
void image::scanline(double P1, double P2, double lim, Mat& disp, Mat& cost, bool dispR){
	double minLeft=0.0;
	double minRight=0.0;
	double minUp=0.0;
	double minDown=0.0;
	//Left path opt
	for(int p= subRH ; p<img_leftRGB.rows-subRH ; p++){				//Excluding boundaries
		for(int q= subRW+1 ; q<img_leftRGB.cols-subRW ; q++){				
			minLeft=0.0;
			minLeft=MinPathCost(left_cost, p,q-1);
			if(!dispR){
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q-d-dispMin>subRW){									// > 0 because in calculation of parameters P1 and P2 for Left path optimization, 
																			//the intensity of the the previous pixel on the left is required, which causes an out of boundry error in case of (q-d-dispMin=0)
						left_cost.at<double>(p,q,d) = costOpt(left_cost, p,q,d, minLeft, 'L', P1, P2, lim);
					}	
				}
			} else {
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q+d+dispMin<img_leftRGB.cols-subRW){									// > 0 because in calculation of parameters P1 and P2 for Left path optimization, 
																			//the intensity of the the previous pixel on the left is required, which causes an out of boundry error in case of (q-d-dispMin=0)
						left_cost.at<double>(p,q,d) = costOpt(left_cost, p,q,d, minLeft, 'L', P1, P2, lim, dispR);
					}	
				}
			}
		}
	}
	
	//Right path opt
	for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){				//Excluding boundaries
		for(int q=img_leftRGB.cols-subRW-2 ; q>=subRW ; q--){				
			minRight=0.0;
			minRight=MinPathCost(right_cost, p,q+1);
			if(!dispR){
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q-d-dispMin>subRW-1)
						right_cost.at<double>(p,q,d) = costOpt(right_cost, p,q,d, minRight, 'R', P1, P2, lim);
				}
			} else {
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q+d+dispMin<img_leftRGB.cols-subRW-1)
						right_cost.at<double>(p,q,d) = costOpt(right_cost, p,q,d, minRight, 'R', P1, P2, lim,dispR);
				}
			}
		}
	}
	
	//Up path opt
	for(int q= subRW; q< img_leftRGB.cols-subRW ; q++){				//Excluding boundaries
		for(int p= subRH+1 ; p<img_leftRGB.rows-subRH ; p++){					
			minUp=0.0;
			minUp=MinPathCost(up_cost, p-1,q);
			if(!dispR){
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q-d-dispMin>subRW-1)
						up_cost.at<double>(p,q,d) = costOpt(up_cost, p,q,d, minUp, 'U', P1, P2, lim);
				}
			} else {
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q+d+dispMin<img_leftRGB.cols-subRW)
						up_cost.at<double>(p,q,d) = costOpt(up_cost, p,q,d, minUp, 'U', P1, P2, lim, dispR);
				}					
			}
		}
	}
	
	//Down Path cost
	for(int q= subRW; q< img_leftRGB.cols-subRW ; q++){			//Excluding boundaries
		for(int p= img_leftRGB.rows-subRH-2 ; p>=subRH ; p--){			
			minDown=0.0;
			minDown=MinPathCost(down_cost, p+1,q);
			if(!dispR){
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q-d-dispMin>subRW-1)
						down_cost.at<double>(p,q,d) = costOpt(down_cost, p,q,d, minDown, 'D', P1, P2, lim);
				}
			} else {
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q+d+dispMin<img_leftRGB.cols-subRW)
						down_cost.at<double>(p,q,d) = costOpt(down_cost, p,q,d, minDown, 'D', P1, P2, lim, dispR);
				}
			}
		}
	}
	
	finalCost(left_cost,right_cost,down_cost,up_cost, final_cost);
	find_disparity(final_cost, disp, cost);
	//subpxEnhance(final_cost,disp);
	
}

/* Minimum path cost of pixel predecessor */
double image::MinPathCost(cv::Mat in, int p, int q){
	//double min_cost=1.79769e+308;
	double min_cost=in.at<double>(p,q,0);
	for(int d=0; d<dispMax-dispMin+1; d++){
		if(in.at<double>(p,q,d)<=min_cost){
			min_cost = in.at<double>(p,q,d);
		}
		//printf("MinPathCost: cost[%d][%d][%d]: %Lf\t\n", p,q,d, in[p][q][d]);
	}
	return min_cost;
}


/* Take the average of all the path cost */
void image::finalCost(cv::Mat Lpath, cv::Mat Rpath, cv::Mat Upath, cv::Mat Dpath, cv::Mat outCost){
	int loc_x=0, loc_y=0, loc_d=0;
	double max=0.0;
	double min = 1.79769e+308;
	for(int d=0; d<dispMax-dispMin+1; d++){
		for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
			for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
				outCost.at<double>(p,q,d) = (Lpath.at<double>(p,q,d) + Rpath.at<double>(p,q,d) + Upath.at<double>(p,q,d) + Dpath.at<double>(p,q,d))/4.0;
				//printf("final_cost(%d,%d,%d): %f\t, left: %f\t, right: %f\t, up: %f\t, down: %f\t\n", p,q,d, outCost.at<double>(p,q,d), Lpath.at<double>(p,q,d), Rpath.at<double>(p,q,d), Upath.at<double>(p,q,d), \
				Dpath.at<double>(p,q,d));
				
				if (max< outCost.at<double>(p,q,d)){
					max=outCost.at<double>(p,q,d);
					loc_x=q;
					loc_y=p;
					loc_d=d;
				}
				if (outCost.at<double>(p,q,d) < min){
					min = outCost.at<double>(p,q,d);
				}
			}
		}
	}
	std::cout<< "min n max value in the whole cost image: " << min << " , " << max << "  x: " << loc_x<< " , y: " << loc_y << " d: " << loc_d <<std::endl;
}



/* Find the final disparity for each pixel based on WTA method */
void image::find_disparity(cv::Mat in, cv::Mat& idisp ,cv::Mat& icost){
	for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
		for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
			//double tmpcost=1.79769e+308;
			double tmpcost=in.at<double>(p,q,0);
			
			for(int d=0; d<dispMax-dispMin+1; d++){
				if(in.at<double>(p,q,d)<= tmpcost){
					tmpcost=in.at<double>(p,q,d);
					idisp.at<float>(p,q)=(float)d+dispMin;				//Check again later to make sure it's correct..
					icost.at<float>(p,q)=tmpcost;
				}
			}
			
		}
	}
	double minv, maxv;
	cv::Point minL, maxL;
	cv::minMaxLoc(icost, &minv,&maxv, &minL, &maxL);
	std::cout<< "minv: " << (float)minv << " maxv: " << (float)maxv << " maxLx: " << maxL.x << " maxL.y: " << maxL.y << std::endl;
}

void image::fMatrix(cv::Mat pixflag, cv::Mat dispL, cv::Mat& FM, int N, double param1, double param2 ){
	vector<cv::Point2f> pointsL;
	vector<cv::Point2f> pointsR;
	int n=0;
	for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
		for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
			if(n<=N){
				if(pixflag.at<float>(p,q)==0){ //reliable pixel
					cv::Point2f p1= Point((float)q, (float)p);
					pointsL.push_back(p1);
					cv::Point2f p2= Point((float)(q-(int)dispL.at<float>(p,q)), (float)p);
					pointsR.push_back(p2);
					n++;
				}
			} else break;
		}
	}

	FM = findFundamentalMat(pointsL, pointsR,FM_RANSAC);
	cout << "fundamental matrix: " << FM << endl;
}

int image::findOutliers(cv::Mat dispL, cv::Mat dispR,cv::Mat& pixflag){
	int n=0;
	for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
		for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
			if(dispL.at<float>(p,q) != dispR.at<float>(p,q-(int)dispL.at<float>(p,q))){
				n++;
				pixflag.at<int>(p,q)=1;
				cout << dispR.at<float>(p,q-(int)dispL.at<float>(p,q)) << endl;
			}
		}
	}
	cout<< n << " outlier!" << endl;
}

void regionVoting(cv::Mat dispL, float ts, float th){
	Mat hist(dispMax+1,CV_32S, Scalar::all(0));
	for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
		for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
			if(pixflag.at<int>(p,q)!=0){ 
					hist=Scalar::all(0);
					int up= supReg.at<int>(p,q,2);
					int down= supReg.at<int>(p,q,3);
					for(int y=p-up;y<=p+down; y++){
						int left= supReg.at<int>(y,q,0);
						int right= supReg.at<int>(y,q,1);
						for(int x=q-left; q<=q+right; q++){
							if(x!=q || y!=p){							//Don't involve the pixel of interest in calculation
								if(pixflag.at<int>(y,x)==0){				//Reliable pixel
									hist.at<int>((int)dispL.at<float>(y,x)) +=1;
								}
							}
							
						}
					}
			}
		}
	}
}

/* Calculating ech path cost to a pixel of interest */
double image::costOpt(cv::Mat in, int p, int q, int d, double preMin, char dir, double param1, double param2, double threshold,bool dispR){
	double cost=0.0;
	std::pair<double,double> P;
	if(!dispR){
		switch (dir){
			case 'L':
				P = calc_param(p,q,p,q-1, p,q-d-dispMin,p,q-d-dispMin-1, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p,q-1,d), in.at<double>(p,q-1,d+1)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p,q-1,d), in.at<double>(p,q-1,d-1)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p,q-1,d), in.at<double>(p,q-1,d-1)+P.first, in.at<double>(p,q-1,d+1)+P.first,preMin+P.second) - preMin;
				}
				/*if(p==4 && q==7 && d==5){
					printf("CostOpt: aggr_cost[%d][%d][%d]= %Lf \t , minLeft:%Lf\t\n , pre_d: %Lf\t, pre_d-1: %Lf\t, param1: %f\t, param2: %f\t\n", p, q, d, 
					aggr_cost[p][q][d], preMin, in[p][q-1][d], in[p][q-1][d-1]+P.first,P.first, P.second);
				}*/
			break;
			
			case 'R':
				P = calc_param(p,q,p,q+1, p,q-d-dispMin,p,q-d-dispMin+1, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p,q+1,d), in.at<double>(p,q+1,d+1)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p,q+1,d), in.at<double>(p,q+1,d-1)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p,q+1,d), in.at<double>(p,q+1,d-1)+P.first, in.at<double>(p,q+1,d+1)+P.first,preMin+P.second) - preMin;
				}
			break;
				
			case 'U':
			
				P = calc_param(p,q,p-1,q, p,q-d-dispMin,p-1,q-d-dispMin, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p-1,q,d), in.at<double>(p-1,q,d+1)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p-1,q,d), in.at<double>(p-1,q,d-1)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p-1,q,d), in.at<double>(p-1,q,d-1)+P.first, in.at<double>(p-1,q,d+1)+P.first,preMin+P.second) - preMin;
				}
			break;
			
			case 'D':
				P = calc_param(p,q,p+1,q, p,q-d-dispMin,p+1,q-d-dispMin, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p+1,q,d), in.at<double>(p+1,q,d+1)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p+1,q,d), in.at<double>(p+1,q,d-1)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p+1,q,d), in.at<double>(p+1,q,d-1)+P.first, in.at<double>(p+1,q,d+1)+P.first,preMin+P.second) - preMin;
				}
			break;
			
			default:
				std::cout << "no direction specified" <<std::endl;
				break;
		}
	} else {					//Right disparity
		switch (dir){
			case 'L':
				P = calc_param(p,q+d+dispMin,p,q+d+dispMin-1, p,q,p,q-1, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p,q-1,d), in.at<double>(p,q-1,d+1)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p,q-1,d), in.at<double>(p,q-1,d-1)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p,q-1,d), in.at<double>(p,q-1,d-1)+P.first, in.at<double>(p,q-1,d+1)+P.first,preMin+P.second) - preMin;
				}
				/*if(p==4 && q==7 && d==5){
					printf("CostOpt: aggr_cost[%d][%d][%d]= %Lf \t , minLeft:%Lf\t\n , pre_d: %Lf\t, pre_d-1: %Lf\t, param1: %f\t, param2: %f\t\n", p, q, d, 
					aggr_cost[p][q][d], preMin, in[p][q-1][d], in[p][q-1][d-1]+P.first,P.first, P.second);
				}*/
			break;
			
			case 'R':
				P = calc_param(p,q+d+dispMin,p,q+d+dispMin+1, p,q,p,q+1, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p,q+1,d), in.at<double>(p,q+1,d+1)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p,q+1,d), in.at<double>(p,q+1,d-1)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p,q+1,d), in.at<double>(p,q+1,d-1)+P.first, in.at<double>(p,q+1,d+1)+P.first,preMin+P.second) - preMin;
				}
			break;
				
			case 'U':
			
				P = calc_param(p,q+d+dispMin,p-1,q+d+dispMin, p,q,p-1,q, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p-1,q,d), in.at<double>(p-1,q,d+1)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p-1,q,d), in.at<double>(p-1,q,d-1)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p-1,q,d), in.at<double>(p-1,q,d-1)+P.first, in.at<double>(p-1,q,d+1)+P.first,preMin+P.second) - preMin;
				}
			break;
			
			case 'D':
				P = calc_param(p,q+d+dispMin,p+1,q+d+dispMin, p,q,p+1,q, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p+1,q,d), in.at<double>(p+1,q,d+1)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p+1,q,d), in.at<double>(p+1,q,d-1)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=aggr_cost.at<double>(p,q,d)+minimum(in.at<double>(p+1,q,d), in.at<double>(p+1,q,d-1)+P.first, in.at<double>(p+1,q,d+1)+P.first,preMin+P.second) - preMin;
				}
			break;
			
			default:
				std::cout << "no direction specified" <<std::endl;
				break;
		}
	}
	return cost;
}

bool image::dispValid(int d){
	return (d < 0 || d >dispMax-dispMin) ? false:true;
}

double image::minimum(double a, double b, double c, double d){
	if(d==-100){
		return a < b ? (a < c ? a : c) : (b < c ? b : c);
	}
	else {
		double tmp1 = a < b ? a : b;
		double tmp2 = c < d ? c : d;
		return tmp1 < tmp2 ? tmp1 : tmp2;
	}
}


/* Calculating some of the constants for the algorithm */
std::pair<double,double> image::calc_param(int x1, int y1, int x2, int y2, int x3, int y3, 
									int x4, int y4, double thr, double const1, 
									double const2){
	
	double param1=0, param2=0;
	std::pair <double,double> p1;
	double leftdiff= colDiffer(img_leftRGB, x1,y1,x2,y2);
	double rightdiff= colDiffer(img_rightRGB, x3,y3,x4,y4);
	if(leftdiff<thr && rightdiff<thr)
	{
		param1=const1;
		param2=const2;
	}
	else if((leftdiff<thr && rightdiff>thr) || (leftdiff>thr && rightdiff<thr))
	{
		param1=const1/4;
		param2=const2/4;
	}
	else  					///FIXME: Here also includes when the values are equal, but in the paper it's just when they are bigger than threshold. Test it to see...
	{
		param1=const1/10;		
		param2=const1/10;
	}
	
	p1 = std::make_pair(param1, param2);
	return p1;
}

double image::findMax(cv::Mat in){
	double max=0.0;
	int x,y;
	for(int d=0; d<dispMax-dispMin+1; d++){
		for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
			for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
				if(max<in.at<double>(p,q,d)){
					max=in.at<double>(p,q,d);
					x=q;
					y=p;
				}
			}
		}
	}
	//std::cout<< "x: " << x << " y: " << y << std::endl;
	return max;
}


void image::subpxEnhance(cv::Mat fcost, cv::Mat& idisp){
	for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
		for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
				int d = idisp.at<float>(p,q)-dispMin;
				//std::cout << "float d: " << idisp.at<float>(p,q)<< " int d: " << d << std::endl;
				if(d>dispMin && d < dispMax-2){
					double val = (fcost.at<double>(p,q,d+1) - fcost.at<double>(p,q,d-1))/(2*(fcost.at<double>(p,q,d+1)+ \
					fcost.at<double>(p,q,d-1) - 2*fcost.at<double>(p,q,d)));
					//std::cout << "val: " << val <<std::endl;
					idisp.at<float>(p,q) = idisp.at<float>(p,q) - (float)val;
					
				}
			}
	}
	//cv::medianBlur(idisp, idisp, 3);
	//cv::GaussianBlur(idisp, idisp, cv::Size(3,3), 3,3, cv::BORDER_DEFAULT);
}
