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


// list of images used for tests
vector<String> imageNames;

void findIntersectionsWithHarris(string imsname);

/********** initialize the list of all image within the folder in parameter ************/
void initProg(String folder){
  

    glob(folder, imageNames);

    for(size_t i = 0; i < imageNames.size(); ++i)
    {
        findIntersectionsWithHarris(imageNames[i]);
	}
}

int isSingleLine(Mat featureWin){


  Mat edges;
  int rows = featureWin.rows;
  int cols = featureWin.cols;
      
      
  vector<Vec4i> p_lines;

  Mat houghLines(rows, cols, 0, Scalar(0)); 

  /// Use Probabilistic Hough Transform
  HoughLinesP( featureWin, p_lines, 1, CV_PI/180, 40, 10,1 );

  /// Draw the segments
  for( size_t i = 0; i < p_lines.size(); i++ )
    {
      Vec4i l = p_lines[i];
      line( houghLines, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,255), 2, CV_AA);
    }

  cout << p_lines.size() <<endl;
  imshow( "lignes detectees", houghLines );
  waitKey(0);
  
  return 0;
}





/** @function cornerHarris_demo */
void findIntersectionsWithHarris(string imsname)
{

  
  Mat img_gray = imread(imsname,0); 

  if(img_gray.empty()){
    cerr << "can not open " << imsname << endl;
  }
    
  int cols = img_gray.cols; 
  int rows = img_gray.rows;
      
  Mat dst, dst_norm, dst_points;
  dst = Mat::zeros( img_gray.size(), CV_32FC1 );
  dst_points = Mat::zeros( img_gray.size(), 0 );
  /// Detector parameters
  int blockSize = 12;
  int apertureSize = 3;
  double k = 0.01;
  int thresh = 90;
  int personalSpace=20;
  
  /// Detecting corners
  cornerHarris( img_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

  /// Normalizing
  normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
  //convertScaleAbs( dst_norm, dst_points );



  /// Drawing a circle around corners
  int nbCorners = 0;
  for( int j = 0; j < rows ; j++ )
    { for( int i = 0; i < cols; i++ )
        {
	  if( (int) dst_norm.at<float>(j,i) > thresh )
            {
	      dst_points.at<uchar>(j,i) = 255; //possible intersection
	      nbCorners++;
	    }
	}
    }

   for( int j = 0; j < rows ; j++ )
    { for( int i = 0; i < cols; i++ )
        {
	  if (dst_points.at<uchar>(j,i) == 255){
	  for(int k = -personalSpace;k<personalSpace;k++)
	    {
	      for(int l = -personalSpace;l<personalSpace;l++)
		{	
		  int a = (i+k);
		  int b = (j+l);
		  if(!((a==i) && (b==j)) && (a >= 0) && (b >= 0) && (b < rows) && (a < cols) )
		    {
		      if(dst_points.at<uchar>(b,a)==255){
			dst_points.at<uchar>(b,a) = 0; //delete other nearby points
			nbCorners--;
		      }
			  
		    }
		}
	    }
	  if( (j>(rows-20)) || (i>(cols-20)) || (j<20) || (i<20)){ //ignore features point near the border 
	    dst_points.at<uchar>(j,i) = 0;
	    nbCorners--;
	  }
	  }
	  
	}
    }
  
  cout <<imsname<<": "<< nbCorners << endl;

  const int featuresSize = nbCorners;
  //Find a better way to store feature points
  Mat featureWin =  Mat::zeros(100, 100, 0);
  Point features[featuresSize];
  int n =0;

  
  /// Showing the result
  imshow( "detected intersections", img_gray );
  
  for( int j = 0; j < rows ; j++ ){
    for( int i = 0; i < cols; i++ ){
      if (dst_points.at<uchar>(j,i) == 255){
	features[n] = Point(i,j);
	n++;
	//circle (img_gray, Point(i,j),8,Scalar(128),2,8,0);
      }
    }
  }

  for(int n=0; n<featuresSize;n++){
    Point featurePoint = features[n];
    int i = featurePoint.x;
    int j = featurePoint.y;
    for(int k = -50;k<50;k++)
      {
	for(int l = -50;l<50;l++)
	  {	
	    int a = (i+k);
	    int b = (j+l);
	    if( (a >= 0) && (b >= 0) && (b < rows) && (a < cols) )
	      {
		featureWin.at<uchar>(l+50,k+50) = img_gray.at<uchar>(b,a);
	      }
	  }
      }
    imshow( "pattern window", featureWin );
    waitKey(0);
    matchingMethod(0,featureWin,featureWin);
    //isSingleLine(featureWin);
    featureWin =  Mat::zeros(100, 100, 0);
  }
  

  
  waitKey(0);
}


void usage(const char *s){
  
  cerr << "Usage: " << s << "imsname th" << endl;
  exit(EXIT_FAILURE);

}

#define param 1
int main( int argc, char** argv){

  if(argc != param+1)
    usage(argv[0]);
  else 
    findIntersectionsWithHarris(argv[1]);

  
  return EXIT_SUCCESS;

}
