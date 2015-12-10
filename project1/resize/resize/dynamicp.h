#ifndef DYNAMICP_H
#define DYNAMICP_H


#include "basic.h"


//	dynamic programming method
class DynamicP {
public:
	IplImage *image;
	int width, height;

	double **f;
	int **pre;
	bool ***ban;

	DynamicP(IplImage *, CvSize);
	~DynamicP();
	void resizeWidth(int);
	void resizeHeight(int);
};


#endif