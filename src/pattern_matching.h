#include <iostream>
#include <cstdlib>
#include <list>

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void matchingMethod( int,cv::Mat img ,cv::Mat srcImg);

int compareMaxs(cv::Vec4f m1, cv::Vec4f m2);
