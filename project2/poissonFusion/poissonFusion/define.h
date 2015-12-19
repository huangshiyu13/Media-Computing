#ifndef DEFINE_H
#define DEFINE_H
#include <opencv2/opencv.hpp>

int dn = 4;
int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};


CvScalar operator + (CvScalar A, CvScalar B) {
	for (int k = 0; k < 3; ++k) A.val[k] += B.val[k];
	return A;
}

CvScalar operator - (CvScalar A, CvScalar B) {
	for (int k = 0; k < 3; ++k) A.val[k] -= B.val[k];
	return A;
}

CvScalar operator * (CvScalar A, double B) {
	for (int k = 0; k < 3; ++k) A.val[k] *= B;
	return A;
}


#endif