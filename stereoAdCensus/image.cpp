#include "image.h" 

using namespace std;
using namespace cv;


image::image(Mat image_leftRGB, Mat image_rightRGB, int dMin, int dMax){
	img_leftRGB=image_leftRGB;
	img_rightRGB=image_rightRGB;
	dispMin = dMin;
	dispMax = dMax;
	s = img_leftRGB.size();
	channels = img_leftRGB.channels();
	cout << "channels: " << channels << endl;
	//std::cout << "Maximum value for double: " << std::numeric_limits<double>::max() << '\n';
	//std::cout << "Maximum value for long double: " << std::numeric_limits<long double>::max() << '\n';
	//std::cout << "height: " << s.height << " , " <<  img_leftRGB.rows<< '\n';
	//std::cout << "width: " << s.width << " , " <<  img_leftRGB.cols<< '\n';
	
	//Initialization of different cost space
	
	//color_left=cv::Mat(s, CV_8UC3, cv::Scalar::all(0));
	//color_right=cv::Mat(s, CV_8UC3,cv::Scalar::all(0));

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
}

void image::reset(){
//	DSI=cv::Scalar::all(0);
	//init_cost=cv::Scalar::all(0);
	//aggr_cost=cv::Scalar::all(0);
	//final_cost=cv::Scalar::all(0);
	//left_cost=cv::Scalar::all(0);
//	right_cost=cv::Scalar::all(0);
//	up_cost=cv::Scalar::all(0);
//	down_cost=cv::Scalar::all(0);
	//HII= cv::Scalar::all(0);
//	VII=cv::Scalar::all(0);
	//sumH= cv::Scalar::all(0);
//	sumV=cv::Scalar::all(0);
	supReg=cv::Scalar::all(0);
	for(int p= 0 ; p<img_leftRGB.rows ; p++){
		for(int q= 0 ; q<img_leftRGB.cols ; q++){
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
cv::Mat* image::costAD(bool dispR){
	cerr << "costAD..." << endl;
	int d,p,q;
	double val =0;
	int drange = dispMax-dispMin+1;
	cv::Mat* DSI = new cv::Mat[drange];
	//int sz[] = {s.height, s.width, dispMax-dispMin+1};
	for(int d=0; d< drange; d++){
		DSI[d] = cv::Mat(s, mytype,cv::Scalar::all(0));
	}
	
	for(d=0;d<drange;d++){
		for(p=subRH;p<img_leftRGB.rows-subRH;p++){					//Rows = height
			for(q=subRW;q<img_leftRGB.cols-subRW;q++){				//cols = width	
				//Left disparity
				val = 0;
				if(!dispR){
					if(q-d-dispMin>subRW-1){
						if(channels==1){
							DSI[d].at<double>(p,q) =(double) (abs(img_leftRGB.at<uchar>(p,q) - img_rightRGB.at<uchar>(p,q-d-dispMin)))/(double)channels;
						}
						else if (channels == 3 || channels == 4) {
							DSI[d].at<double>(p,q)= (double)((abs(img_leftRGB.at<cv::Vec3b>(p,q).val[0] - img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[0])) + 
							(abs(img_leftRGB.at<cv::Vec3b>(p,q).val[1] - img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[1])) +
							(abs(img_leftRGB.at<cv::Vec3b>(p,q).val[2] - img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[2])))/3;
						} 
						else {
							cerr << "Unhandled number of channels " << endl;

						}

						/*DSI.at<double>(p,q,d)= (double)((abs(img_leftRGB.at<cv::Vec3b>(p,q).val[0] - img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[0])) + 
						(abs(img_leftRGB.at<cv::Vec3b>(p,q).val[1] - img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[1])) +
						(abs(img_leftRGB.at<cv::Vec3b>(p,q).val[2] - img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[2])))/3;*/

						/*printf ("b: %f\t, g: %u\t, r: %u\t, bR: %u\t, gR: %u\t , rR: %u\t" , (double)img_leftRGB.at<cv::Vec3b>(p,q).val[0], img_leftRGB.at<cv::Vec3b>(p,q).val[1],
						img_leftRGB.at<cv::Vec3b>(p,q).val[2], img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[0],
						img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[1],img_rightRGB.at<cv::Vec3b>(p,q-d-dispMin).val[2]);*/
					}
				}
				else{
					//Right disparity
					if(q+d+dispMin<img_leftRGB.cols-subRW){
						
						if(channels==1){
							DSI[d].at<double>(p,q) = (double) (abs(img_leftRGB.at<uchar>(p,q+d+dispMin)- img_rightRGB.at<uchar>(p,q)))/(double)channels;
						}
						else if (channels == 3 || channels == 4){
							DSI[d].at<double>(p,q)=(double)((abs(img_leftRGB.at<cv::Vec3b>(p,q+d+dispMin).val[0] - img_rightRGB.at<cv::Vec3b>(p,q).val[0])) + 
							(abs(img_leftRGB.at<cv::Vec3b>(p,q+d+dispMin).val[1] - img_rightRGB.at<cv::Vec3b>(p,q).val[1])) +
							(abs(img_leftRGB.at<cv::Vec3b>(p,q+d+dispMin).val[2] - img_rightRGB.at<cv::Vec3b>(p,q).val[2])))/3;
						}
						else {
							cerr << "Unhandled number of channels " << endl;
						}
					}
				}
			}
		}
	}
	return DSI;
//	cout << "DSI: " << DSI.at<double>(100,100,10) << endl;
}

/* Encoding each pixel local structure */
void image::costCensus(int winX, int winY, int left){
	uint64_t census;
	uint64_t bit=0;
	int shifts;
	cv::Mat left_gray, right_gray;
	if(channels == 3 || channels == 4){
		cvtColor(img_leftRGB,left_gray,CV_BGR2GRAY);
		cvtColor(img_rightRGB,right_gray,CV_BGR2GRAY);
	} else {
		left_gray = img_leftRGB;
		right_gray = img_rightRGB;

	}
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
	cerr << "hamdist..." << endl;
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
void image::initCost(cv::Mat* DSI, double lam_AD, double lam_census){
	//int sz[] = {s.height, s.width, dispMax-dispMin+1};
	//init_cost=cv::Mat(3, sz, mytype,cv::Scalar::all(0));
	std::cerr << "initCost..." <<std::endl;
	int p,q,d;
	//cout << DSI[1](cv::Rect(50,50,5,5)) << endl;
	for(d=0;d<dispMax-dispMin+1;d++){
		for(p= subRH ; p<img_leftRGB.rows-subRH ; p++){					//Rows = height
			for(q= subRW ; q<img_leftRGB.cols-subRW ; q++){				//cols = width
				DSI[d].at<double>(p,q)= 2.0 - exp(-DSI[d].at<double>(p,q)/lam_AD) - exp(-(double)census_hamming[p][q][d]/lam_census);
				if(DSI[d].at<double>(p,q) > 2.0)
					abort();
			}
		}
	}
	//cout << DSI[1](cv::Rect(50,50,5,5)) << endl;
	//std::cout<< "init_cost(100,200,10): " << init_cost.at<double>(100,200,10) << std::endl;
	//printf("DSI: %f\t\t , census: %f\t\t , cost: %f\t\t\n ", DSI[0][0][0], (double)census_hamming[0][0][0],init_cost[0][0][0] );
}

/* Building the cross-based region for each pixel */
void image::line_segment(double colLim1, double colLim2, double distLim1, double distLim2, bool dispR){
	cerr << "line_segment..." << endl;
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
void image::aggregateCost(cv::Mat* icost){
	cerr << "aggregateCost..." << endl;
	int counter=1;
	int iter=4;
	//int sz[] = {s.height, s.width, dispMax-dispMin+1};
	//cv::Mat HII(3, sz , mytype, cv::Scalar::all(0));
	//cv::Mat VII(3, sz, mytype, cv::Scalar::all(0));
	int drange = dispMax-dispMin+1;
	sumH = new cv::Mat[drange];
	for(int d=0; d<drange ;d++){
		sumH[d]= cv::Mat(s, mytype, cv::Scalar::all(0));
	}
	//cv::Mat sumV(3, sz, mytype, cv::Scalar::all(0));
	while(counter<=iter){
		switch(counter){
			case 1:
				std::cout<<"1"<<std::endl;	
				IImage(icost, sumH, 'H');
				for(int d=0; d<drange ;d++){
					icost[d] = cv::Scalar::all(0);
				}
				finalSum(sumH, icost, 'H', counter);
				for(int d=0; d<drange ;d++){
					sumH[d] = cv::Scalar::all(0);
				}
				IImage(icost, sumH, 'V');
				for(int d=0; d<drange ;d++){
					icost[d] = cv::Scalar::all(0);
				}
				finalSum(sumH, icost,'V', counter);
				//VII = cv::Scalar::all(0);
				//std::cout<< "sumV(100,200,10): " << sumV.at<double>(100,200,10) << std::endl;
			break;
			case 2:
				std::cout<<"2"<<std::endl;
				for(int d=0; d<drange ;d++){
					sumH[d] = cv::Scalar::all(0);
				}
				IImage(icost, sumH, 'V');
				for(int d=0; d<drange ;d++){
					icost[d] = cv::Scalar::all(0);
				}
				finalSum(sumH, icost,'V', counter);
				for(int d=0; d<drange ;d++){
					sumH[d] = cv::Scalar::all(0);
				}
				IImage(icost, sumH, 'H');
				for(int d=0; d<drange ;d++){
					icost[d] = cv::Scalar::all(0);
				}
				finalSum(sumH, icost, 'H', counter);
				//HII = cv::Scalar::all(0);
				//std::cout<< "sumH(100,200,10): " << sumH.at<double>(100,200,10) << std::endl;
			break;
			case 3:
				std::cout<<"3"<<std::endl;
				for(int d=0; d<drange ;d++){
					sumH[d] = cv::Scalar::all(0);
				}
				IImage(icost, sumH,'H');
				for(int d=0; d<drange ;d++){
					icost[d] = cv::Scalar::all(0);
				}
				finalSum(sumH, icost, 'H',counter);
				for(int d=0; d<drange ;d++){
					sumH[d] = cv::Scalar::all(0);
				}
				IImage(icost, sumH, 'V');
				for(int d=0; d<drange ;d++){
					icost[d] = cv::Scalar::all(0);
				}
				finalSum(sumH,icost,'V',counter);
				//VII = cv::Scalar::all(0);
				//std::cout<< "sumV(100,200,10): " << sumV.at<double>(100,200,10) << std::endl;
			break;
			case 4:
				std::cout<< "4"<<std::endl;
				for(int d=0; d<drange ;d++){
					sumH[d] = cv::Scalar::all(0);
				}
				IImage(icost,sumH, 'V');
				for(int d=0; d<drange ;d++){
					icost[d] = cv::Scalar::all(0);
				}
				finalSum(sumH, icost,'V',counter);
				for(int d=0; d<drange ;d++){
					sumH[d] = cv::Scalar::all(0);
				}
				IImage(icost,sumH, 'H');
				for(int d=0; d<drange ;d++){
					icost[d] = cv::Scalar::all(0);
				}
				finalSum(sumH, icost, 'H',counter);
				//HII = cv::Scalar::all(0);
				//std::cout<< "sumH(100,200,10): " << sumH.at<double>(100,200,10) << std::endl;
			break;
			default:
				std::cout<< "iteration out of range" <<std::endl;
			break;
		}
		counter++;
	}

	//aggr_cost=cv::Mat(3, sz, mytype, cv::Scalar::all(0));	
	for (int i=0; i<drange; i++){
		sumH[i] = cv::Scalar::all(0);
		sumH[i] = icost[i];
	}
	//finalSum(sumH, aggr_cost, 'C',  counter-1);
	//std::cout<< "aggr_cost(100,200,10): " << aggr_cost.at<double>(100,200,10) << std::endl;
		
}
/* Calculating Integral Image */
void image::IImage(cv::Mat* in, cv::Mat* out, char dir){
	cerr << "IImage..." << endl;
	double max=0.0;
	switch (dir){
		case 'H':
			for(int d=0; d<dispMax-dispMin+1; d++){
				for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
					for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
						out[d].at<double>(p,q) =in[d].at<double>(p,q) + (q-1 < subRW ? 0: out[d].at<double>(p,q-1));
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
						out[d].at<double>(p,q)= in[d].at<double>(p,q) +( p -1 < subRH ? 0 : out[d].at<double>(p-1,q));
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
void image::finalSum(cv::Mat* in, cv::Mat* out, char dir, int count){
	cerr << "finalSum..." << endl;
	switch (dir){
		case 'H':
			for(int d=0; d<dispMax-dispMin+1; d++){
				for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
					for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
						int left= supReg.at<int>(p,q,0);				//left arm
						int right= supReg.at<int>(p,q,1);				//right arm
						out[d].at<double>(p,q) = in[d].at<double>(p,q+right)- (q-left-1 < subRW ? 0 : in[d].at<double>(p,q-left-1));
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
						out[d].at<double>(p,q) = in[d].at<double>(p+down,q)-(p-up-1 < subRH ? 0 : in[d].at<double>(p-up-1,q));
						
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
	double color = 0;
	if (channels == 1){
		color = (double) abs(in.at<uchar>(x1,y1)-in.at<uchar>(x2,y2));
	}
	else if (channels == 2){
		color = (double) std::max(abs(in.at<cv::Vec3b>(x1,y1)[0]-in.at<cv::Vec3b>(x2,y2)[0]), abs(in.at<cv::Vec3b>(x1,y1)[1]-in.at<cv::Vec3b>(x2,y2)[1]));
	}
	else if(channels == 3 || channels == 4) {
		color = (double) std::max(std::max(abs(in.at<cv::Vec3b>(x1,y1)[0]-in.at<cv::Vec3b>(x2,y2)[0]), abs(in.at<cv::Vec3b>(x1,y1)[1]-in.at<cv::Vec3b>(x2,y2)[1])), 
					abs(in.at<cv::Vec3b>(x1,y1)[2]-in.at<cv::Vec3b>(x2,y2)[2]));
	}
	else {
		cerr << "Unhandled case for number of channels more than 3..." << endl;
	}
					
	return color;
}

/* Scanline optimization from 4 direction: LRUD */
cv::Mat* image::scanline(double P1, double P2, double lim, Mat& disp, Mat& cost, bool dispR){	
	cerr << "scanline..." << endl;
//	int sz[] = {s.height, s.width, dispMax-dispMin+1};
	int drange = dispMax-dispMin+1;
	cv::Mat* final_cost = new cv::Mat[drange];
	semi_cost = new cv::Mat[drange];
	for(int i=0; i < dispMax-dispMin+1; i++){
		semi_cost[i]= cv::Mat(s, mytype, cv::Scalar::all(0));	
		final_cost[i] = cv::Mat(s,mytype, cv::Scalar::all(0));
	}
	for(int d=0; d< drange; d++){
		for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
			for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
				if(q==subRW || q==img_leftRGB.cols-subRW-1 || p==subRH || p==img_leftRGB.rows-subRH-1) semi_cost[d].at<double>(p,q)=sumH[d].at<double>(p,q);
			}
		}
	}
	double minLeft=0.0;
	double minRight=0.0;
	double minUp=0.0;
	double minDown=0.0;
	//Left path opt
	for(int p= subRH ; p<img_leftRGB.rows-subRH ; p++){				//Excluding boundaries
		for(int q= subRW+1 ; q<img_leftRGB.cols-subRW ; q++){				
			minLeft=0.0;
			minLeft=MinPathCost(semi_cost, p,q-1);
			if(!dispR){
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q-d-dispMin>subRW){									// > 0 because in calculation of parameters P1 and P2 for Left path optimization, 
																			//the intensity of the the previous pixel on the left is required, which causes an out of boundry error in case of (q-d-dispMin=0)
						semi_cost[d].at<double>(p,q) = costOpt(semi_cost, p,q,d, minLeft, 'L', P1, P2, lim);	
						final_cost[d].at<double>(p,q) += (semi_cost[d].at<double>(p,q)/4.0);
					}	
				}
			} else {
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q+d+dispMin<img_leftRGB.cols-subRW){									// > 0 because in calculation of parameters P1 and P2 for Left path optimization, 
																			//the intensity of the the previous pixel on the left is required, which causes an out of boundry error in case of (q-d-dispMin=0)
						semi_cost[d].at<double>(p,q) = costOpt(semi_cost, p,q,d, minLeft, 'L', P1, P2, lim, dispR);
						final_cost[d].at<double>(p,q) += (semi_cost[d].at<double>(p,q)/4.0);
					}	
				}
			}
		}
	}
	
	for(int i=0; i < dispMax-dispMin+1; i++){
		semi_cost[i] = cv::Scalar::all(0);
	}
	//Right path opt
	for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){				//Excluding boundaries
		for(int q=img_leftRGB.cols-subRW-2 ; q>=subRW ; q--){				
			minRight=0.0;
			minRight=MinPathCost(semi_cost, p,q+1);
			if(!dispR){
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q-d-dispMin>subRW-1)
						semi_cost[d].at<double>(p,q) = costOpt(semi_cost, p,q,d, minRight, 'R', P1, P2, lim);
						final_cost[d].at<double>(p,q) += (semi_cost[d].at<double>(p,q)/4.0);
				}
			} else {
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q+d+dispMin<img_leftRGB.cols-subRW-1)
						semi_cost[d].at<double>(p,q) = costOpt(semi_cost, p,q,d, minRight, 'R', P1, P2, lim,dispR);
						final_cost[d].at<double>(p,q) += (semi_cost[d].at<double>(p,q)/4.0);
				}
			}
		}
	}
	for(int i=0; i < dispMax-dispMin+1; i++){
		semi_cost[i] = cv::Scalar::all(0);
	}
	//Up path opt
	for(int q= subRW; q< img_leftRGB.cols-subRW ; q++){				//Excluding boundaries
		for(int p= subRH+1 ; p<img_leftRGB.rows-subRH ; p++){					
			minUp=0.0;
			minUp=MinPathCost(semi_cost, p-1,q);
			if(!dispR){
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q-d-dispMin>subRW-1)
						semi_cost[d].at<double>(p,q) = costOpt(semi_cost, p,q,d, minUp, 'U', P1, P2, lim);
						final_cost[d].at<double>(p,q) += (semi_cost[d].at<double>(p,q)/4.0);
				}
			} else {
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q+d+dispMin<img_leftRGB.cols-subRW)
						semi_cost[d].at<double>(p,q) = costOpt(semi_cost, p,q,d, minUp, 'U', P1, P2, lim, dispR);
						final_cost[d].at<double>(p,q) += (semi_cost[d].at<double>(p,q)/4.0);
				}					
			}
		}
	}
	
	for(int i=0; i < dispMax-dispMin+1; i++){
		semi_cost[i] = cv::Scalar::all(0);
	}
	//Down Path cost
	for(int q= subRW; q< img_leftRGB.cols-subRW ; q++){			//Excluding boundaries
		for(int p= img_leftRGB.rows-subRH-2 ; p>=subRH ; p--){			
			minDown=0.0;
			minDown=MinPathCost(semi_cost, p+1,q);
			if(!dispR){
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q-d-dispMin>subRW-1)
						semi_cost[d].at<double>(p,q) = costOpt(semi_cost, p,q,d, minDown, 'D', P1, P2, lim);
						final_cost[d].at<double>(p,q) += (semi_cost[d].at<double>(p,q)/4.0);
				}
			} else {
				for(int d=0; d<dispMax-dispMin+1; d++){
					if(q+d+dispMin<img_leftRGB.cols-subRW)
						semi_cost[d].at<double>(p,q) = costOpt(semi_cost, p,q,d, minDown, 'D', P1, P2, lim, dispR);
						final_cost[d].at<double>(p,q) += (semi_cost[d].at<double>(p,q)/4.0);
				}
			}
		}
	}	
//	finalCost(left_cost,right_cost,down_cost,up_cost, final_cost);
	find_disparity(final_cost, disp, cost);
	return final_cost;
	//subpxEnhance(final_cost,disp);
	
}

/* Minimum path cost of pixel predecessor */
double image::MinPathCost(cv::Mat* in, int p, int q){
	//double min_cost=1.79769e+308;
	double min_cost=in[0].at<double>(p,q);
	for(int d=0; d<dispMax-dispMin+1; d++){
		if(in[d].at<double>(p,q)<=min_cost){
			min_cost = in[d].at<double>(p,q);
		}
		//printf("MinPathCost: cost[%d][%d][%d]: %Lf\t\n", p,q,d, in[p][q][d]);
	}
	return min_cost;
}


/* Take the average of all the path cost */
void image::finalCost(cv::Mat Lpath, cv::Mat Rpath, cv::Mat Upath, cv::Mat Dpath, cv::Mat& outCost){
	cerr << "finalCost..." << endl;
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
void image::find_disparity(cv::Mat* in, cv::Mat& idisp ,cv::Mat& icost){
	cerr << "find_disparity..." << endl;
	for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
		for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
			//double tmpcost=1.79769e+308;
			double tmpcost=in[0].at<double>(p,q);
			
			for(int d=0; d<dispMax-dispMin+1; d++){
				if(in[d].at<double>(p,q)<= tmpcost){
					tmpcost=in[d].at<double>(p,q);
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
				if(pixflag.at<float>(p,q)==0){ 							//reliable pixel
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


/* Detecting and labeling outliers */
int image::findOutliers(cv::Mat dispL, cv::Mat dispR,cv::Mat& pixflag, float f, float B){
	cerr << "findOutliers..." << endl;
	int n=0;
	cv::Mat Ql= (Mat_<float>(4,4) << 1,0,0, -(img_leftRGB.cols/2),0,1,0,-(img_leftRGB.rows/2),0,0,0,f,0,0, -1/B, 0);
	    cv::Mat Qr= (Mat_<float>(4,4) << 1,0,0, -(img_leftRGB.cols/2),0,1,0,-(img_leftRGB.rows/2),0,0,0,f,0,0, -1/B, 0);
	cv::Mat T= (Mat_<float>(4,4) << 1,0,0, B,0,1,0,0,0,0,1,0,0,0, 0, 1);
	for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
		for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
			//cout<< "dispL: " << dispL.at<float>(p,q) << " dispR: " << dispR.at<float>(p,q-(int)dispL.at<float>(p,q)) << endl;
			if(dispL.at<float>(p,q) != dispR.at<float>(p,q-(int)dispL.at<float>(p,q))){
				//cout<< "dispL: " << dispL.at<float>(p,q) << " dispR: " << dispR.at<float>(p,q-(int)dispL.at<float>(p,q)) << endl;
				n++;
				int occluded = labelOut(Ql, (float)p, (float)q, dispL.at<float>(p,q), dispR.at<float>(p,q-(int)dispL.at<float>(p,q)));
				if(occluded == 1){
					pixflag.at<int>(p,q)=1;				//Occluded
				//cout << dispR.at<float>(p,q-(int)dispL.at<float>(p,q)) << endl;
				}
				else pixflag.at<int>(p,q)=-1;			//Mistmatch
			}
			
		}
	}
	cout<< n << " outlier!" << endl;
	
}

int image::labelOut(cv::Mat Ql, float pl,float ql, float dl, float dr){
	cv::Mat p1= (Mat_<float>(4,1) << ql,pl,dl,1);
	cv::Mat q1= (Mat_<float>(4,1) << ql-dl, pl, dr, 1);
	cv::Mat p2= (Mat_<float>(4,1) << (ql-dl)+dr, pl, dr, 1);
	cv::Mat res1,res2, res3,res4;
	gemm(p1,Ql, 1, 0, 0, res1, GEMM_1_T + GEMM_2_T);
	//gemm(res1,T, 1, 0, 0, res2, 0);
	//gemm(res2, Qr,1, 0, 0, res3, 0);
	gemm(Ql,p2, 1, 0, 0, res2, 0);
	gemm(res1,res2, 1, 0, 0, res3, 0);
	
	gemm(Ql,p1, 1, 0, 0, res4, 0);
	float mgt1 = (float) sqrt((res2.at<float>(0)*res2.at<float>(0))+ (res2.at<float>(1)*res2.at<float>(1)) 
				+ (res2.at<float>(2)*res2.at<float>(2))+ (res2.at<float>(3)*res2.at<float>(3)));
	float mgt2 = (float) sqrt((res4.at<float>(0)*res4.at<float>(0))+ (res4.at<float>(1)*res4.at<float>(1)) 
				+ (res4.at<float>(2)*res4.at<float>(2))+ (res4.at<float>(3)*res4.at<float>(3)));
	
	float RE1 = res3.at<float>(0);
	float RE2 = mgt1 * mgt2;
	//cout << "res4: " << res4 << " res3: " << res3 << " mgt1: " <<mgt1 << " mgt2: " << mgt2 << " multi: " << RE2 << " " << RE1<< endl;
	//cout << " abs: " << abs(RE1-RE2) << endl;
	if(abs(RE1-RE2) < 2){											//Threshold? (Float calculation difference?)
		//cout << " Mismatch" << endl;
		return 0;
	}
	else {
		//cout << " Occluded!" << endl;
		return 1;
	}
	
	//cv::Mat zero = Mat::zeros(1,1,CV_32F);
	//cv::Mat diff = res4 != zero;
	// Equal if no elements disagree
	//bool equal = cv::countNonZero(diff) == 0;
	/*if(equal){
		cout << res4 << " Mismatch" << endl;
	}*/
	//else cout << "Occluded" << endl;
}

void image::interpolate(cv::Mat img, cv::Mat& disp, cv::Mat pixflag){
	cerr << "interpolate..." << endl;
	for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
		for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
			
			if(pixflag.at<int>(p,q)==1){ 				//Occluded
				float min_disp = 10000000;
				float min1, min2;
				for(int j=-2; j<3; j++){
					
					min1 = std::min(((p-2 < subRH || q+j > img_leftRGB.cols-subRW-1 || q+j < subRW || pixflag.at<int>(p-2,q+j)!=0) ? 20000000 : disp.at<float>(p-2,q+j)), 
									((p+2 > img_leftRGB.rows-subRH-1 || q+j > img_leftRGB.cols-subRW-1 || q+j < subRW || pixflag.at<int>(p+2,q+j)!=0) ? 20000000 : disp.at<float>(p+2,q+j)));
					if(min1 < min_disp){
						min_disp = min1;
					}
				}
				for (int i=-1; i<2; i++){
					min1 = std::min(((p+i < subRH || p+i > img_leftRGB.rows-subRH-1 || q-2 < subRW || pixflag.at<int>(p+i,q-2)!=0) ? 20000000 : disp.at<float>(p+i,q-2)),
									((p+i < subRH || p+i > img_leftRGB.rows-subRH-1 || q+2 > img_leftRGB.cols-subRW-1 || pixflag.at<int>(p+i,q+2)!=0) ? 20000000 : disp.at<float>(p+i, q+2)));
					if(min1 < min_disp){
						min_disp = min1;
					}
				}
				if(min_disp == 10000000 || min_disp == 20000000) ; //cout << "Very large disp! " << endl;
				else disp.at<float>(p,q) = min_disp;							//lowest disparity from 16 nearest reliable pixels
			}
			else if(pixflag.at<int>(p,q)== -1){ 				//Mismatch
				int x,y, tempx, tempy;
				Mat gimg;
				int min_diff = 10000000;
				int temp_diff;
				int diff1, diff2;
				unsigned char I2, I3;
				if( channels == 3 || channels == 4){
					cvtColor(img, gimg, CV_BGR2GRAY);
				} else {
					gimg = img;
				}
				unsigned char I1 = gimg.at<uchar>(p,q);
				for(int j=-2; j<3; j++){
					I2 =  gimg.at<uchar>(p-2,q+j);
					I3 =  gimg.at<uchar>(p+2,q+j);
					diff1 = (p-2 < subRH || q+j < subRW || q+j > img_leftRGB.cols-subRW-1 || pixflag.at<int>(p-2,q+j)!=0) ? 20000000 : abs((int)I1 - I2);
					diff2 = (p+2 > img_leftRGB.rows-subRH-1 || q+j < subRW || q+j > img_leftRGB.cols-subRW-1 || pixflag.at<int>(p+2,q+j)!=0) ? 20000000 : abs((int)I1 - I3);
					if(diff1 < diff2){
						temp_diff = diff1;
						tempx = q+j;
						tempy = p-2;
					} else {
						temp_diff = diff2;
						tempx = q+j;
						tempy = p+2;
					}
					if(temp_diff < min_diff ){
						min_diff = temp_diff;
						x = tempx;
						y = tempy;
					}
				}
				for (int i=-1; i<2; i++){
					I2 = gimg.at<uchar>(p+i,q-2);
					I3 = gimg.at<uchar>(p+i,q+2);
					diff1 = (q-2 < subRW || p+i < subRH || p+i > img_leftRGB.rows-subRH-1 || pixflag.at<int>(p+i,q-2)!=0) ? 20000000 : abs((int)I1 - I2);
					diff2 = (q+2 > img_leftRGB.cols-subRW-1 || p+i < subRH || p+i > img_leftRGB.rows-subRH-1 || pixflag.at<int>(p+i,q+2)!=0) ? 20000000 : abs((int)I1 - I3);
					if(diff1 < diff2){
						temp_diff = diff1;
						tempx = q-2;
						tempy = p+i;
					} else {
						temp_diff = diff2;
						tempx = q+2;
						tempy = p+i;
					}
					if(temp_diff < min_diff){
						min_diff = temp_diff;
						x = tempx;
						y = tempy;
					}
				}
				if(min_diff == 20000000 || min_diff == 10000000) ;//cout << "Very large intensity difference! " << endl;
				else disp.at<float>(p,q) = disp.at<float>(y,x);
			}
			
		}
	}
}

void image::border(cv::Mat disp, cv::Mat& grad){
	cerr << "border..." << endl;
	//cv::Mat Gx = (Mat_<float>(3,3) << 1,0,-1, 2,0,-2, 1,0,-1);
	//cv::Mat Gy = (Mat_<float>(3,3) << 1,2,1, 0,0,0, -1,-2,-1);
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	Scharr( disp, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	//Sobel( disp, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_x, abs_grad_x );

	/// Gradient Y
	Scharr( disp, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	//Sobel( disp, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( grad_y, abs_grad_y );

	/// Total Gradient (approximate)
	addWeighted( abs_grad_x, 1, abs_grad_y, 1, 0, grad );
	
}

void image::discAdjust(cv::Mat& disp, cv::Mat* fcost, cv::Mat mask){
	cerr << "discAdjust..." << endl;
	//std::list<Point> pt;
	int n=0;
	for(int p=subRH+1 ; p<img_leftRGB.rows-subRH-1 ; p++){					
		for(int q= subRW+1 ; q<img_leftRGB.cols-subRW-1 ; q++){
			n=0;
			if(mask.at<int>(p,q) != 0){	
				if(mask.at<int>(p,q-1) ==0){
					n++;
					//cout << " Left Not On Edge " << endl;
					//Point p1= Point(q-1,p);
					//pt.push_back(p1);
				}
				if(mask.at<int>(p,q+1) == 0){
					n++;
				}
				//cout << "size: " << pt.size() << endl;
				if(n==2){
					//cout << " 2 points found!" << endl;
				
					if(fcost[(int)disp.at<float>(p,q-1)].at<double>(p,q-1) < fcost[(int)disp.at<float>(p,q)].at<double>(p,q)){
						disp.at<float>(p,q) = disp.at<float>(p,q-1);
					}
					if(fcost[(int)disp.at<float>(p,q+1)].at<double>(p,q+1) < fcost[(int)disp.at<float>(p,q)].at<double>(p,q)){
						disp.at<float>(p,q) = disp.at<float>(p,q+1);
					}
					
				}
				
			}
		}
	}
	
}


/* Iterative region voting */
void image::regionVoting(cv::Mat& dispL, cv::Mat& pixflag, int TS, double TH,int iter){
	cerr << "regionVoting..." << endl;
	int n=0;
	Mat hist(dispMax-dispMin+1,1,CV_32SC1, Scalar::all(0));
	int reliables=0;				//Number of reliable pixels
	
	while(n<iter){
		for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
			for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
				if(pixflag.at<int>(p,q)!=0){
					//cout << "p,q " << p << " , " << q << endl;
						reliables=0;
						hist=Scalar::all(0);
						int up= supReg.at<int>(p,q,2);
						int down= supReg.at<int>(p,q,3);
						//cout << "up: " << up << " Down: " << down << endl;
						for(int y=p-up;y<=p+down; y++){
							int left= supReg.at<int>(y,q,0);
							int right= supReg.at<int>(y,q,1);
							//cout <<  y << " , " << q <<  " left: " << left << " right: " << right << endl;
							for(int x=q-left; x<=q+right; x++){
								//if(x!=q || y!=p){							//Don't involve the pixel of interest in calculation -> NOT necessary the next 'if' excludes it by itself
									if(pixflag.at<int>(y,x)==0){				//Reliable pixel
										reliables++;
										hist.at<int>((int)dispL.at<float>(y,x)) +=1;
									}
									//cout << "reliable: " << reliables << endl;
								//}
							}
						}
						int newd= mostVote(hist);
						//cout << "hist: " << hist << '\t' << " most vote: " << newd << endl;
						if(reliables > TS && ( ((double)hist.at<int>(newd,0)/reliables) > TH)) {
							//cout <<  p << " , " << q << " original disp: " << dispL.at<float>(p,q) << "  updated disp: " << (float) newd +  dispMin<< endl;
							dispL.at<float>(p,q) = (float) newd + dispMin;
							pixflag.at<int>(p,q) = 0;
						}
				}
			} 
		}
		n++;
	}
}

int image::mostVote(cv::Mat hist){
	int votes = hist.at<int>(0,0);
	int index=0;
	for (int i=0; i<hist.rows; i++){
		if(hist.at<int>(i,0)>=votes){
			votes = hist.at<int>(i,0);
			index=i;
		}
	}
	return index;
}


/* Calculating ech path cost to a pixel of interest */
double image::costOpt(cv::Mat* in, int p, int q, int d, double preMin, char dir, double param1, double param2, double threshold,bool dispR){
	double cost=0.0;
	std::pair<double,double> P;
	if(!dispR){
		switch (dir){
			case 'L':
				P = calc_param(p,q,p,q-1, p,q-d-dispMin,p,q-d-dispMin-1, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p,q-1), in[d+1].at<double>(p,q-1)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p,q-1), in[d-1].at<double>(p,q-1)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p,q-1), in[d-1].at<double>(p,q-1)+P.first, in[d+1].at<double>(p,q-1)+P.first,preMin+P.second) - preMin;
				}
				/*if(p==4 && q==7 && d==5){
					printf("CostOpt: aggr_cost[%d][%d][%d]= %Lf \t , minLeft:%Lf\t\n , pre_d: %Lf\t, pre_d-1: %Lf\t, param1: %f\t, param2: %f\t\n", p, q, d, 
					aggr_cost[p][q][d], preMin, in[p][q-1][d], in[p][q-1][d-1]+P.first,P.first, P.second);
				}*/
			break;
			
			case 'R':
				P = calc_param(p,q,p,q+1, p,q-d-dispMin,p,q-d-dispMin+1, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p,q+1), in[d+1].at<double>(p,q+1)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p,q+1), in[d-1].at<double>(p,q+1)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p,q+1), in[d-1].at<double>(p,q+1)+P.first, in[d+1].at<double>(p,q+1)+P.first,preMin+P.second) - preMin;
				}
			break;
				
			case 'U':
			
				P = calc_param(p,q,p-1,q, p,q-d-dispMin,p-1,q-d-dispMin, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p-1,q), in[d+1].at<double>(p-1,q)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p-1,q), in[d-1].at<double>(p-1,q)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p-1,q), in[d-1].at<double>(p-1,q)+P.first, in[d+1].at<double>(p-1,q)+P.first,preMin+P.second) - preMin;
				}
			break;
			
			case 'D':
				P = calc_param(p,q,p+1,q, p,q-d-dispMin,p+1,q-d-dispMin, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p+1,q), in[d+1].at<double>(p+1,q)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p+1,q), in[d-1].at<double>(p+1,q)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p+1,q), in[d-1].at<double>(p+1,q)+P.first, in[d+1].at<double>(p+1,q)+P.first,preMin+P.second) - preMin;
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
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p,q-1,d), in[d+1].at<double>(p,q-1)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p,q-1), in[d-1].at<double>(p,q-1)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p,q-1), in[d-1].at<double>(p,q-1)+P.first, in[d+1].at<double>(p,q-1)+P.first,preMin+P.second) - preMin;
				}
				/*if(p==4 && q==7 && d==5){
					printf("CostOpt: aggr_cost[%d][%d][%d]= %Lf \t , minLeft:%Lf\t\n , pre_d: %Lf\t, pre_d-1: %Lf\t, param1: %f\t, param2: %f\t\n", p, q, d, 
					aggr_cost[p][q][d], preMin, in[p][q-1][d], in[p][q-1][d-1]+P.first,P.first, P.second);
				}*/
			break;
			
			case 'R':
				P = calc_param(p,q+d+dispMin,p,q+d+dispMin+1, p,q,p,q+1, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p,q+1), in[d+1].at<double>(p,q+1)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p,q+1), in[d-1].at<double>(p,q+1)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p,q+1), in[d-1].at<double>(p,q+1)+P.first, in[d+1].at<double>(p,q+1)+P.first,preMin+P.second) - preMin;
				}
			break;
				
			case 'U':
			
				P = calc_param(p,q+d+dispMin,p-1,q+d+dispMin, p,q,p-1,q, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p-1,q), in[d+1].at<double>(p-1,q)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p-1,q), in[d-1].at<double>(p-1,q)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p-1,q), in[d-1].at<double>(p-1,q)+P.first, in[d+1].at<double>(p-1,q)+P.first,preMin+P.second) - preMin;
				}
			break;
			
			case 'D':
				P = calc_param(p,q+d+dispMin,p+1,q+d+dispMin, p,q,p+1,q, threshold, param1, param2);
				if(!dispValid(d-1) && dispValid(d+1)){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p+1,q), in[d+1].at<double>(p+1,q)+P.first,preMin+P.second)- preMin;
				}
				else if (dispValid(d-1) && !(dispValid(d+1))){
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p+1,q), in[d-1].at<double>(p+1,q)+P.first,preMin+P.second)- preMin;
				}
				else {
					cost=sumH[d].at<double>(p,q)+minimum(in[d].at<double>(p+1,q), in[d-1].at<double>(p+1,q)+P.first, in[d+1].at<double>(p+1,q)+P.first,preMin+P.second) - preMin;
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

double image::findMax(cv::Mat* in){
	double max=0.0;
	int x,y;
	for(int d=0; d<dispMax-dispMin+1; d++){
		for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
			for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
				if(max<in[d].at<double>(p,q)){
					max=in[d].at<double>(p,q);
					x=q;
					y=p;
				}
			}
		}
	}
	//std::cout<< "x: " << x << " y: " << y << std::endl;
	return max;
}


void image::subpxEnhance(cv::Mat* fcost, cv::Mat& idisp){
	cerr << "subpxEnhance..." << endl;
	for(int p=subRH ; p<img_leftRGB.rows-subRH ; p++){					
		for(int q= subRW ; q<img_leftRGB.cols-subRW ; q++){
				int d = idisp.at<float>(p,q)-dispMin;
				//std::cout << "float d: " << idisp.at<float>(p,q)<< " int d: " << d << std::endl;
				if(d>0 && d < dispMax-dispMin){
					double val = (fcost[d+1].at<double>(p,q) - fcost[d-1].at<double>(p,q))/(2*(fcost[d+1].at<double>(p,q)+
					fcost[d-1].at<double>(p,q) - 2*fcost[d].at<double>(p,q)));
				//	std::cout << "old disp: " << idisp.at<float>(p,q) <<std::endl;
					idisp.at<float>(p,q) = idisp.at<float>(p,q) - (float)val;
				//	std::cout << "new disp: " << idisp.at<float>(p,q) <<std::endl;
				}
			}
	}
	cv::medianBlur(idisp, idisp, 3);
	//cv::GaussianBlur(idisp, idisp, cv::Size(3,3), 3,3, cv::BORDER_DEFAULT);
}
