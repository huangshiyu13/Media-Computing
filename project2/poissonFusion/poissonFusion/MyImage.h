#ifndef MYIMAGE_H
#define MYIMAGE_H


#include <string>
#include "define.h"
using namespace std;

class MyImage {
public:
	string winName;
	IplImage *oriImage;
	IplImage *image;
	
	int width, height;
	CvSize size;

	MyImage(const char *filename, string winName){
		image = cvLoadImage(filename);
		oriImage = cvCloneImage(image);
		width = image->width;
		height = image->height;
		size = cvSize(width,height);
		this->winName = winName;
		
	}

	~MyImage() {
		cvDestroyWindow(winName.c_str());
		cvReleaseImage(&image);
	}

	void show() {
		cvShowImage(winName.c_str(), image);
	}

	void createWindow() {
		cvNamedWindow(winName.c_str());
		show();
	}

	CvScalar getColor(int x, int y) {
		return cvGet2D(image, y, x);
	}

	double getColor(int x, int y, int k) {
		return getColor(x, y).val[k];
	}

	void setColor(int x, int y, CvScalar c) {
		cvSet2D(image, y, x, c);
	}
};
#endif