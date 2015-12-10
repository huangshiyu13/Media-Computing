#ifndef BASIC_H
#define BASIC_H


#include <opencv2/opencv.hpp>


#define SQR(x) ((x)*(x))


CvScalar operator + (const CvScalar, const CvScalar);
CvScalar operator * (const CvScalar, const double);
double dist(CvPoint, CvPoint);
double dist(CvScalar, CvScalar);


#endif