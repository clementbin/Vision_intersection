#include <iostream>
#include <cstdlib>
#include <list>
#include <time.h>
#include <math.h>

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "pattern_matching.h"

using namespace cv;
using namespace std;

int vect_product (const int x1, const int y1, const int x2, const int y2){
	return (x1 * y2) - (x2 * y1);
}
float scalar_product(const float x1, const float y1, const float x2, const float y2){
	return (x1 * x2) - (y1 * y2);
}
float norme (const float x1, const float y1, const float x2, const float y2){
	return sqrt((x2-x1) * (x2-x1) + (y2-y1)*(y2-y1));
}
int is_intersect (Vec4i v, Vec4i u){
	int ux1 = u[0];
	int uy1 = u[1];
	int ux2 = u[2];
	int uy2 = u[3];
	int vx1 = v[0];
	int vy1 = v[1];
	int vx2 = v[2];
	int vy2 = v[3];

	if ((ux1 == ux2)&&(uy1 == uy2)&&(ux1 == vx2)&&(uy1 == vy2) || (ux1 == ux2)&&(uy1 == uy2)&&(ux1 == vx1)&&(uy1 == vy1)  
			|| (ux1 == vx2)&&(uy1 == vy2)&&(ux1 == vx1)&&(uy1 == vy1)  || (vx2 == ux2)&&(vy2 == uy2)&&(vx2 == vx1)&&(vy2 == vy1) )
		return 1;
	else if (vect_product(ux1 - ux2, uy1 - uy2, vx1 - vx2, vy1 - vy2) == 0)
		return 0;
	else if ((vect_product(ux1 - ux2, uy1 - uy2, ux1 - vx2, uy1 - vy2) * vect_product(ux1 - ux2, uy1 - uy2, ux1 - vx1, uy1 - vy1)) > 0)
		return 0;
	else if ((vect_product(vx1 - vx2, vy1 - vy2, vx1 - ux2, vy1 - uy2) * vect_product(vx1 - vx2, vy1 - vy2, vx1 - ux1, vy1 - uy1)) > 0)
		return 0;
	else 
		return 1;
}
float cos_angle_rad (Vec4i v, Vec4i u){
	float ux1 = u[0];
	float uy1 = u[1];
	float ux2 = u[2];
	float uy2 = u[3];
	float vx1 = v[0];
	float vy1 = v[1];
	float vx2 = v[2];
	float vy2 = v[3];
	return scalar_product(ux2 - ux1, uy2 - uy1, vx2 - vx1, vy2 - vy1) / (norme(ux1, uy1, ux2, uy2) * norme(vx1, vy1, vx2, vy2));
}

int main()
{
	return;
}
