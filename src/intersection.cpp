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
  
  erode(mask,mask , getStructuringElement(MORPH_ELLIPSE, Size(10,10))); //might be useful...
  dilate(mask,mask, getStructuringElement(MORPH_ELLIPSE, Size(10,10)));  
 
  imshow("tmpmask",mask);

}


void process(const char* imsname, int th){
  Mat img = imread(imsname, 1);
  Mat img_gray;
  Mat mask;
  int cols = img.cols; 
  int rows = img.rows; 

  pitch_mask(img, mask);

  if(img.empty()){
    cerr << "can not open " << imsname << endl;
  }
    
  imshow("source", img);
  cvtColor( img, img_gray, CV_BGR2GRAY );

  
  for (int i = 0; i<rows; i++){
    for(int j = 0; j<cols; j++){
      img_gray.at<uchar>(i,j) = (mask.at<uchar>(i,j)==255 ) ? img_gray.at<uchar>(i,j) : 0;
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
      if(img_gray.at<uchar>(i,j)>mean+50 && mask.at<uchar>(i,j)==255){
	img_gray.at<uchar>(i,j) = 255;
      }
      else{
	img_gray.at<uchar>(i,j) = 0;
	}
	}
}

    
  imshow("source+mask", img_gray);

  Mat edges;

  Canny( img_gray, edges, 50, 200, 3 );
  vector<Vec4i> p_lines;

  Mat houghLines(rows, cols, 0, Scalar(0)); 
  // cvtColor( edges, houghLines, COLOR_GRAY2BGR );

  /// 2. Use Probabilistic Hough Transform
  HoughLinesP( edges, p_lines, 1, CV_PI/180, th, 30, 10 );

  /// Show the result
  for( size_t i = 0; i < p_lines.size(); i++ )
     {
       Vec4i l = p_lines[i];
       line( houghLines, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,255), 3, CV_AA);
     }

   imshow( "lignes detectees", houghLines );

  
  waitKey();
     
}

void usage(const char *s){
  
  cerr << "Usage: " << s << " imsname th" << endl;
  exit(EXIT_FAILURE);

}

#define param 2
int main( int argc, char** argv){

  if(argc != (param+1))
    usage(argv[0]);
  process(argv[1],atoi(argv[2]));
  return EXIT_SUCCESS;

}
