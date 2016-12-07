#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;


void process(const char* imsname, int thmag){
	Mat img = imread(imsname, 0);
	int cols = img.cols; 
	int rows = img.rows; 
	if(img.empty()){
		cerr << "can not open " << imsname << endl;
    }
    
	imshow("source", img);
    Mat houghImg(rows,cols,CV_8UC3,Scalar(0,0,0));
    int mn = (int) sqrt(cols*cols + rows*rows);
    Mat accumulator(mn, 180, 0,Scalar(0));

    for (int i = 0; i<rows; i++){
    	for(int j = 0; j<cols; j++){
    		if (img.at<uchar>(i,j)>thmag){
    			for (int th = 0; th<180; th++){
    				int rho = abs(i*cos(th*CV_PI/180.0)+j*sin(th*CV_PI/180.0));
    				accumulator.at<uchar>(rho,th)++;
    			}
    		}
    	}
    }

	imshow("accumulateur", accumulator);

    vector<int> maxs;
    vector<int> rhoMaxs;
    vector<int> thMaxs;

    // Handmade thresholding of accumulator 
    for (int i = 0; i<mn; i++){
    	for(int j = 0; j<180; j++){
	    	if(accumulator.at<uchar>(i,j)<105){ // the 105 value is experimental
	    		accumulator.at<uchar>(i,j)=0;
	    	}	
    	}
    }

    // Get the local maxs
    for (int i = 0; i<mn; i++){
    	for(int j = 0; j<180; j++){
	    	if(accumulator.at<uchar>(i,j)!=0){
	    		maxs.push_back(accumulator.at<uchar>(i,j));
	    		rhoMaxs.push_back(i);
	    		thMaxs.push_back(j);
	    	}	
    	}
    }

    // draw the lines...
    for(int i = 0 ; i<maxs.size();i++){


    	int rhoMax = rhoMaxs[i];
    	int thMax = thMaxs[i];

	    for (int x = 0; x < rows; x++){
	    	int y = (int)(rhoMax - x*cos(thMax*CV_PI/180.0))/sin(thMax*CV_PI/180.0);
	    	houghImg.at<Vec3b>(x,y)[0] = 0;
	    	houghImg.at<Vec3b>(x,y)[1] = 0;
	    	houghImg.at<Vec3b>(x,y)[2] = 255;

	    }
	}

	imshow("lignes detectees", houghImg);

	waitKey();
     
}

void usage(const char *s){
  
  cerr << "Usage: " << s << " imsname th-mag" << endl;
  exit(EXIT_FAILURE);

}

#define param 2
int main( int argc, char** argv){

  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1],atoi(argv[2]));
  return EXIT_SUCCESS;

}
