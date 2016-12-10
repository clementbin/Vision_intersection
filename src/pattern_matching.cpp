#include "pattern_matching.h"

using namespace cv;
using namespace std;


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
