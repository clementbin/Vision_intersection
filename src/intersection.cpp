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
     
    Mat dst, cdst;
	Canny(img, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

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
	   line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
	}
	imshow("source", img);
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
