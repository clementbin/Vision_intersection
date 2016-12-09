#include <iostream>
#include <cstdlib>
#include <list>

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

#define iLowHGREEN 22//limit = 24
#define iHighHGREEN 60//limit = 59

#define iLowSGREEN 50//limit = 50
#define iHighSGREEN 215//limit = 215

#define iLowVGREEN 40//limit = 40
#define iHighVGREEN 175//limit = 175

String imageNames[16] = {"001-rgb.png","0001-rgb.png","34-rgb.png","074-rgb.png","083-rgb.png","094-rgb.png","099-rgb.png","101-rgb.png","147-rgb.png","156-rgb.png","157-rgb.png","164-rgb.png","194-rgb.png","205-rgb.png","264-rgb.png","268-rgb.png"};

String templates[3] = {"corner_pattern.png","cross_pattern.png","t_pattern.png"};

void matchingMethod( int,Mat img ,Mat srcImg)
{
  
  vector<Mat> results;
  
  int match_method = 5; // Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED
  
  Mat img_display,templ;
  
  templ = imread("../data/"+templates[0],0);
 
  for(int i=0; i<3; i++){
  /// Source image to display
    Mat result;
  img.copyTo( img_display );
  templ = imread("../data/"+templates[i],0);
  /// Create the result matrix
  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

   result.create( result_rows, result_cols, 0 );
  

  /// Do the Matching and Normalize

  matchTemplate( img, templ, result, match_method );

  results.push_back(result);
  }

  Mat result = results[0].clone();
  Mat featureLoc(result.rows, result.cols, 0, Scalar(0)); 

   for (int n = 1;n<3;n++){
      for (int i = 0; i<result.rows;i++){
	for (int j = 0;j<result.cols;j++){
	  if(results[n].at<float>(i,j)>result.at<float>(i,j)){
	    result.at<float>(i,j) = results[n].at<float>(i,j);
	    featureLoc.at<uchar>(i,j) = n;
	      }
	}
      }
      }

  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    
  vector<Vec4i> maxPoints;
  int cornerNb = 0;
  int crossNb = 0;
  int tNb = 0;
  for (int i = 0; i<result.rows;i++){
    for (int j = 0;j<result.cols;j++){
      if (result.at<float>(i,j)>0.95){
	if(featureLoc.at<uchar>(i,j)==0 && cornerNb<3){
	  maxPoints.push_back(Vec4i(j,i,result.at<float>(i,j),featureLoc.at<uchar>(i,j)));
	  cornerNb++;
	}
	else if(featureLoc.at<uchar>(i,j)==1 && crossNb<3){
	    maxPoints.push_back(Vec4i(j,i,result.at<float>(i,j),featureLoc.at<uchar>(i,j)));
	    crossNb++;
	}
	else if(featureLoc.at<uchar>(i,j)==2 && tNb<3){
	  maxPoints.push_back(Vec4i(j,i,result.at<float>(i,j),featureLoc.at<uchar>(i,j)));
	  tNb++;
	}
      }
    }
  }
  
  cout << maxPoints.size() << endl; // Maybe should do a sort of maxPoints and just keep the 6 highests... or highest for each color...

  /// Show me what you got
  for (Vec4i v : maxPoints){
    if(v[3]==0) // Blue == Corner
      rectangle( srcImg, Point(v[0], v[1]), Point( v[0] + templ.cols , v[1] + templ.rows ), Scalar(255,0,0), 2, 8, 0 );
    else if(v[3]==1) // Green == Cross
      rectangle( srcImg, Point(v[0], v[1]), Point( v[0] + templ.cols , v[1] + templ.rows ), Scalar(0,255,0), 2, 8, 0 );
    else // Red == T
      rectangle( srcImg, Point(v[0], v[1]), Point( v[0] + templ.cols , v[1] + templ.rows ), Scalar(0,0,255), 2, 8, 0 );
  }
  imshow( "pattern detection", srcImg );
  
  return;
}

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

void process(const char* imsname, int th){

  for(int i = 0; i< 16; i++)
    {
      Mat img = imread("../data/" +  imageNames[i] , 1);
      Mat img_gray;
      Mat mask;
      int cols = img.cols; 
      int rows = img.rows; 

      pitch_mask(img, mask);

      if(img.empty()){
	cerr << "can not open " << imsname << endl;
      }
    
      //imshow("source", img);

      cvtColor( img, img_gray, CV_BGR2GRAY );
  
      for (int i = 0; i<rows; i++){
	for(int j = 0; j<cols; j++){
	  img_gray.at<uchar>(i,j) = (mask.at<uchar>(i,j)==255 ) ? img_gray.at<uchar>(i,j) : 0;
	  int mean = 0;
	  int cnt = 0;
	  for(int k = -5;k<6;k++)
	    {
	      for(int l = -5;l<6;l++)
		{	
		  int a = (i+k);
		  int b = (j+l);
		  if( (a >= 0) && (b >= 0) && (a < rows) && (b < cols) )
		    {
		      mean += img_gray.at<uchar>(a,b);
		      cnt++;
		    }
		}
	    }
	  mean = mean/cnt;
	  
	  if(img_gray.at<uchar>(i,j)>mean+60 && mask.at<uchar>(i,j)==255){
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

      /// Use Probabilistic Hough Transform
      HoughLinesP( edges, p_lines, 1, CV_PI/180, th, 30, 30 );

      /// Show the result
      for( size_t i = 0; i < p_lines.size(); i++ )
	{
	  Vec4i l = p_lines[i];
	  line( houghLines, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,255), 5, CV_AA);
	}

      //  dilate(houghLines,houghLines, getStructuringElement(MORPH_ELLIPSE, Size(10,10)));  
      //   erode(houghLines,houghLines , getStructuringElement(MORPH_ELLIPSE, Size(10,10))); //might be useful...
  
 

      imshow( "lignes detectees", houghLines );
  
      matchingMethod(0,houghLines,img);
      imwrite("houghLines164.png", houghLines);
  
      waitKey();
    }
     
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
