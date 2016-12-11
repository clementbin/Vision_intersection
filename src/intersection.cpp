#include <iostream>
#include <cstdlib>
#include <list>
#include <time.h>

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "pattern_matching.h"

using namespace cv;
using namespace std;

#define iLowHGREEN 22//limit = 24
#define iHighHGREEN 60//limit = 59

#define iLowSGREEN 50//limit = 50
#define iHighSGREEN 215//limit = 215

#define iLowVGREEN 40//limit = 40
#define iHighVGREEN 175//limit = 175

// list of images used for tests
String imageNames[16] = {"001-rgb.png","0001-rgb.png","34-rgb.png","074-rgb.png","083-rgb.png","094-rgb.png","099-rgb.png","101-rgb.png","147-rgb.png","156-rgb.png","157-rgb.png","164-rgb.png","194-rgb.png","205-rgb.png","264-rgb.png","268-rgb.png"};


/********** Compute a mask of the football field (green) ************/
void pitch_mask(const Mat& source_img, Mat& mask){
  
  int cols = source_img.cols; 
  int rows = source_img.rows;
  Mat hsv_img;
  
  cvtColor(source_img, hsv_img, COLOR_BGR2HSV); 
  
  inRange(hsv_img, Scalar(iLowHGREEN, iLowSGREEN, iLowVGREEN), Scalar(iHighHGREEN, iHighSGREEN, iHighVGREEN), mask);

  medianBlur(mask,mask,23);

  erode(mask,mask , getStructuringElement(MORPH_ELLIPSE, Size(10,10))); //might be useful...
  
  imshow("tmpmask",mask);
}

/********** find the intersections for a given input image ************/
void findIntersections(string imsname){

 
      Mat img = imread(imsname , 1); 

      if(img.empty()){
	cerr << "can not open " << imsname << endl;
      }
    
      Mat img_gray;
      Mat mask;
      int cols = img.cols; 
      int rows = img.rows; 


      // compute the mask
      pitch_mask(img, mask);


      cvtColor( img, img_gray, CV_BGR2GRAY );

      // Apply the mask to the image
      for (int i = 0; i<rows; i++){
	for(int j = 0; j<cols; j++){
	  img_gray.at<uchar>(i,j) = (mask.at<uchar>(i,j)==255 ) ?  img_gray.at<uchar>(i,j) : 0;
	  float mean = 0;
	  float cnt = 0;
	  //find local maxs
	  for(int k = -6;k<7;k++)
	    {
	      for(int l = -6;l<7;l++)
		{	
		  int a = (i+k);
		  int b = (j+l);
		  if( (a >= 0) && (b >= 0) && (a < rows) && (b < cols) )
		    {
		      mean += (float)img_gray.at<uchar>(a,b);
		      cnt++;
		    }
		}
	    }
	  mean = mean/cnt;

	  //binarization
	  if(mask.at<uchar>(i,j)==255 && img_gray.at<uchar>(i,j)>mean+60){
	    img_gray.at<uchar>(i,j) = 255;
	  }
	  else{
	     img_gray.at<uchar>(i,j) = 0;
	     }
	}
      }

      imshow("source+mask", img_gray);

       
      /*Detect the contours and apply Probabilistic Hough Transform to it*/

      Mat edges;
      
      Canny( img_gray, edges, 50, 200, 3 );
      
      
      vector<Vec4i> p_lines;

      Mat houghLines(rows, cols, 0, Scalar(0)); 

      /// Use Probabilistic Hough Transform
      HoughLinesP( edges, p_lines, 1, CV_PI/180, 30, 30, 30 );

      /// Draw the segments
      for( size_t i = 0; i < p_lines.size(); i++ )
	{
	  Vec4i l = p_lines[i];
	  line( houghLines, Point(l[0]-5, l[1]), Point(l[2]+5, l[3]), Scalar(255,255,255), 5, CV_AA);
	}

      imshow( "lignes detectees", houghLines );

      //Call the matching method, to try to match patterns with detected lines
      matchingMethod(0,houghLines,img);
      
      imwrite("houghLines164.png", houghLines);
  
      waitKey();
     
}

void usage(const char *s){
  
  cerr << "Usage: " << s << "imsname" << endl;
  exit(EXIT_FAILURE);

}

#define param 1
int main( int argc, char** argv){

  if(argc > param+1)
    usage(argv[0]);
  else if (argc == param+1){
    findIntersections(argv[1]);
  }
  else{
    for(int i = 0; i< 16; i++)
      {
	//	clock_t begin = clock();
	findIntersections("../data/" + imageNames[i]);
	//	clock_t end = clock();
	//	cout << "time spent:" << (double) (end - begin) << endl;
      }
  }
  
  return EXIT_SUCCESS;

}
