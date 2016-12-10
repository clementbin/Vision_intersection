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

    
  vector<Vec4f> maxPoints;
  
  for (int i = 0; i<result.rows;i++){
    for (int j = 0;j<result.cols;j++){
      if (result.at<float>(i,j)>0.96){
	  maxPoints.push_back(Vec4f(j,i,result.at<float>(i,j),featureLoc.at<uchar>(i,j)));
      }
    }
  }
  
  // cout << maxPoints.size() << endl; // Maybe should do a sort of maxPoints and just keep the 6 highests... or highest for each color...
  
  sort(maxPoints.begin(), maxPoints.end(), compareMaxs);
  
  /// Show found patterns - take the 2 best (if any) from each pattern
  int cntCorner=0,cntCross=0,cntT=0;
  for (Vec4f v : maxPoints){
    cout << v[2] << endl;
    if(v[3]==0 && cntCorner<2){ // Blue == Corner
      rectangle( srcImg, Point(v[0], v[1]), Point( v[0] + templ.cols , v[1] + templ.rows ), Scalar(255,0,0), 2, 8, 0 );
      cntCorner++;
    }
    else if(v[3]==1 && cntCross<2){ // Green == Cross
      rectangle( srcImg, Point(v[0], v[1]), Point( v[0] + templ.cols , v[1] + templ.rows ), Scalar(0,255,0), 2, 8, 0 );
      cntCross++;
    }
    else if(v[3]==2 && cntT<2){// Red == T
      rectangle( srcImg, Point(v[0], v[1]), Point( v[0] + templ.cols , v[1] + templ.rows ), Scalar(0,0,255), 2, 8, 0 );
      cntT++;
    }
  }
  
  imshow( "pattern detection", srcImg );
  
  return;
}

int compareMaxs(Vec4f m1, Vec4f m2){
  return (m1[2]>m2[2]);
}
