#ifndef DEFINE_H
#define DEFINE_H


#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
#define SQR(x) ((x)*(x))
typedef vector<int> CutPath;
enum changeType{ReduceW,ReduceH, IncreaseW, IncreaseH};
int LXN = 3,LYN  = 3;
int LX[] = {-1, 0, 1};
int LY[] = {-1, 0, 1};
CvScalar myCV2D[1500][1500];
double f[1500][1500];
int pre[1500][1500];

CvScalar operator + (const CvScalar c0, const CvScalar c1) {
	CvScalar c2;
	c2.val[0] = c0.val[0] + c1.val[0];
	c2.val[1] = c0.val[1] + c1.val[1];
	c2.val[2] = c0.val[2] + c1.val[2];
	return c2;
}

CvScalar operator * (const CvScalar c0, const double d) {
	CvScalar c2;
	c2.val[0] = c0.val[0] * d;
	c2.val[1] = c0.val[1] * d;
	c2.val[2] = c0.val[2] * d;
	return c2;
}

double dist(CvPoint a, CvPoint b) {
	return sqrt(SQR(a.x - b.x) + SQR(a.y - b.y));
}

double dist(CvScalar a, CvScalar b) {
	double tot = 0;
	for (int k = 0; k < 3; ++k) tot += SQR(a.val[k] - b.val[k]);
	return sqrt(tot);
}

#endif