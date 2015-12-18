#include "basic.h"


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