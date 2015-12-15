#ifndef DYNAMICP_H
#define DYNAMICP_H


#include "basic.h"

class DynamicP {
public:
	IplImage *image;
	int width, height;
	int maxWidth,maxHeight;
	double **f;
	int **pre;
	bool ***ban;

	DynamicP(IplImage *_image, CvSize size) {
		image = _image;
		width = size.width;
		height = size.height;
	}

	~DynamicP() {
		for (int x = 0; x < width; ++x) {
			delete[] f[x];
			delete[] pre[x];
		}
		delete[] f;
		delete[] pre;
	}

	void resizeWidth(int);
	void resizeHeight(int);
};


#endif