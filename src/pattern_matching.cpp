#include "pattern_matching.h"

using namespace cv;
using namespace std;

#define TEMPLATE_NB 8

void matchingMethod( int,Mat img ,Mat srcImg)
{
  
  vector<Mat> results;
  
  int match_method = 3; // Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED
  
  Mat img_display,templ;
 
  for(int i=1; i<TEMPLATE_NB+1; i++){
    for(int j=0; j<3;j++){
      
      Mat result;
      img.copyTo( img_display );
      string patternName;
      if(j==0)
	patternName = string("../data/Patterns/l") + to_string(i) + ".png";
      else if(j==1)
	patternName = string("../data/Patterns/x") + to_string(i) + ".png";
      else
	patternName = string("../data/Patterns/t") + to_string(i) + ".png";

      templ = imread(patternName,0);
      /// Create the result matrix
      int result_cols =  img.cols - templ.cols + 1;
      int result_rows = img.rows - templ.rows + 1;
    
      result.create( result_rows, result_cols, 0 );
    
      /// Do the Matching and Normalize

      matchTemplate( img, templ, result, match_method );

      results.push_back(result);
    }
  }

  Mat result = results[0].clone();
  Mat resultMult = results[0].clone();
  Mat featureLoc(result.rows, result.cols, 0, Scalar(0)); 

  for (int n = 1;n<results.size();n++){
    for (int i = 0; i<result.rows;i++){
      for (int j = 0;j<result.cols;j++){
	if(results[n].at<float>(i,j)>result.at<float>(i,j)){
	  result.at<float>(i,j) = results[n].at<float>(i,j);
	  featureLoc.at<uchar>(i,j) = n;
	}
	resultMult.at<float>(i,j) *= (1.0 + results[n].at<float>(i,j));
      }
    }
  }

  //imshow("seuil",result);
  //imshow("seuilMult", resultMult);

  normalize( result, result, 0, 10, NORM_MINMAX, -1, Mat() );

    
  vector<Vec4f> maxPoints;
  
  for (int i = 0; i<result.rows;i++){
    for (int j = 0;j<result.cols;j++){
      if (result.at<float>(i,j)>8.0){
	maxPoints.push_back(Vec4f(j,i,result.at<float>(i,j),featureLoc.at<uchar>(i,j)));
      }
    }
  }
  
  // cout << maxPoints.size() << endl; // Maybe should do a sort of maxPoints and just keep the 6 highests... or highest for each color...
  
  sort(maxPoints.begin(), maxPoints.end(), compareMaxs);
  
  /// Show found patterns - take the 2 best (if any) from each pattern
  int cntCorner=0,cntCross=0,cntT=0;
  int maxNbVals = 2;
  for (int i=0;i<maxPoints.size();i++){
    Vec4f v = maxPoints[i];
    if(((int)v[3]%3)==0 && cntCorner<maxNbVals){ // Blue == Corner
      rectangle( srcImg, Point(v[0], v[1]), Point( v[0] + templ.cols , v[1] + templ.rows ), Scalar(255,0,0), 2, 8, 0 );
      
      cout << v << endl;
      cntCorner++;
    }
    else if(((int)v[3]%3)==1 && cntCross<maxNbVals){ // Green == Cross
      rectangle( srcImg, Point(v[0], v[1]), Point( v[0] + templ.cols , v[1] + templ.rows ), Scalar(0,255,0), 2, 8, 0 );
      cout << v << endl;
      cntCross++;
    }
    else if(((int)v[3]%3)==2 && cntT<maxNbVals){// Red == T
      rectangle( srcImg, Point(v[0], v[1]), Point( v[0] + templ.cols , v[1] + templ.rows ), Scalar(0,0,255), 2, 8, 0 );
      cntT++;    
      cout << v << endl;
    }
  }
  
  imshow( "pattern detection", srcImg );
  
  return;
}

int compareMaxs(Vec4f m1, Vec4f m2){
  return (m1[2]>m2[2]);
}
