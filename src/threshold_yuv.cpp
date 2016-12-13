/**
 * @file HoughLines_Demo.cpp
 * @brief Demo code for Hough Transform
 * @author OpenCV team
 */

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

/// Global variables

/** General variables */
Mat src;
Mat src_gray;

int max_trackbar = 255;

int y_trackbar = 0;
int ymax_trackbar = 35;
int u_trackbar = 113;
int umax_trackbar = 218;
int v_trackbar = 84;
int vmax_trackbar = 171;


/// Function Headers
void Threshold_YUV( int, void* );

/**
 * @function main
 */
int main( int, char** argv )
{
   /// Read the image
   src = imread( argv[1], IMREAD_COLOR );

   if( src.empty() )
     {
       return -1;
     }

   /// Create Trackbars for Thresholds

   namedWindow( "thresholding", WINDOW_AUTOSIZE );
   createTrackbar("Y-min", "thresholding", &y_trackbar, max_trackbar, Threshold_YUV);
   createTrackbar("Y-max", "thresholding", &ymax_trackbar, max_trackbar, Threshold_YUV);
   createTrackbar("U-min", "thresholding", &u_trackbar, max_trackbar, Threshold_YUV);
   createTrackbar("U-max", "thresholding", &umax_trackbar, max_trackbar, Threshold_YUV);
   createTrackbar("V-min", "thresholding", &v_trackbar, max_trackbar, Threshold_YUV);
   createTrackbar("V-max", "thresholding", &vmax_trackbar, max_trackbar, Threshold_YUV);
 
   /// Initialize
   Threshold_YUV(0,0);
   waitKey(0);
   return 0;
}

void Threshold_YUV( int, void*){
    inRange(src, Scalar(y_trackbar, u_trackbar, v_trackbar), Scalar(ymax_trackbar, umax_trackbar, vmax_trackbar), src_gray);
    imshow("thresholding",src_gray);
}

