#ifndef DEFINE_H
#define DEFINE_H
#include <opencv2/opencv.hpp>

//	maximum image size
#define MAX_IMAGE_WIDTH 1200
#define MAX_IMAGE_HEIGHT 800

//	blending gradient or blending pixel
#define BLENDING_GRADIENT

//	get coordinate in bakImg using coordinate in cutImg
#define TARX(x) (posx-msk->cx+(x))
#define TARY(y) (posy-msk->cy+(y))

//	blending coefficient
#define SOURCE_RATIO 0.7

//	get inverse of a color
#define INVCOLOR(x) (CV_RGB(255, 255,255)-x)


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