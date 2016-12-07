#include <iostream>
#include <cstdlib>

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

#define iLowHGREEN 20
#define iHighHGREEN 80

#define iLowSGREEN 0
#define iHighSGREEN 255

#define iLowVGREEN 0
#define iHighVGREEN 255


void pitch_mask(const Mat& source_img, Mat& mask){
  
  int cols = source_img.cols; 
  int rows = source_img.rows;
  Mat hsv_img;
  
  cvtColor(source_img, hsv_img, COLOR_BGR2HSV); 
  
  inRange(hsv_img, Scalar(iLowHGREEN, iLowSGREEN, iLowVGREEN), Scalar(iHighHGREEN, iHighSGREEN, iHighVGREEN), mask);
  
  erode(mask,mask , getStructuringElement(MORPH_ELLIPSE, Size(23,23))); //might be useful...
  dilate(mask,mask, getStructuringElement(MORPH_ELLIPSE, Size(23,23)));  
 
  imshow("tmpmask",mask);

}


void process(const char* imsname, int thmag){
  Mat img = imread(imsname, 1);
  Mat img_gray;
  Mat mask;
  int cols = img.cols; 
  int rows = img.rows; 

  pitch_mask(img, mask);

  if(img.empty()){
    cerr << "can not open " << imsname << endl;
  }
    

  cvtColor( img, img_gray, CV_BGR2GRAY );

  
  for (int i = 0; i<rows; i++){
    for(int j = 0; j<cols; j++){
      //img_gray.at<uchar>(i,j) = (img_gray.at<uchar>(i,j)>90 && mask.at<uchar>(i,j)==255 ) ? 255 : 0;
      int mean = 0;
      for(int k = -5;k<6;k++)
	{
	  for(int l = -5;l<6;l++)
	    {	
		  int a = (i+k);
		  int b = (j+l);
		  if( (a >= 0) && (b >= 0) && (a < rows) && (b < cols) )
		    {
		      mean += img_gray.at<uchar>(a,b);
		    }
	    }
	}
      mean = mean/121;
      if(img_gray.at<uchar>(i,j)>mean+60 && mask.at<uchar>(i,j)==255){
	img_gray.at<uchar>(i,j) = 255;
      }
      else{
	img_gray.at<uchar>(i,j) = 0;
      }
    }
  }

    
  imshow("source", img_gray);

  Mat dst, cdst;
  Canny(img_gray, dst, 50, 200, 3);
  cvtColor(dst, cdst, CV_GRAY2BGR);
  // int erosion_size = 10;
  // dilate(dst,dst, getStructuringElement(MORPH_RECT, Size(erosion_size,1)));  
  // erode(dst,dst , getStructuringElement(MORPH_RECT, Size(erosion_size,1)));
 
  
  Mat accumulator((int)sqrt(rows*rows+cols*cols),CV_PI,0,Scalar(0));
  vector<Vec2f> lines;
  HoughLines(dst, lines, 1, CV_PI/180, thmag, 0, 0 );

  for( size_t i = 0; i < lines.size(); i++ )
    {
      float rho = lines[i][0], theta = lines[i][1];
      accumulator.at<uchar>(rho,theta)++;
      Point pt1, pt2;
      double a = cos(theta), b = sin(theta);
      double x0 = a*rho, y0 = b*rho;
      pt1.x = x0 + 1000*(-b);
      pt1.y = y0 + 1000*(a);
      pt2.x = x0 - 1000*(-b);
      pt2.y = y0 - 1000*(a);
      line( cdst, pt1, pt2, Scalar(0,0,255), 1, CV_AA);
    }
  imshow("source", img_gray);
  imshow("lignes detectees", cdst);
  imshow("contours", dst);
  imshow("accumulateur", accumulator);

  
  
  
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
