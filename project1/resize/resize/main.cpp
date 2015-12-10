#include <ctime>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include "basic.h"
#include "costflow.h"
#include "dynamicp.h"
#include "shortestpath.h"

//	remove command window
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

//	detect size of image boundary which is used to resize
#define RESIZE_DETECT_SIZE 10


//	window name
const char *windowName = "Content-Aware Image Resizing";

//	main image
IplImage *image;

//	size of window, size of image
CvSize maxSize, nowSize;

//	is leisure, resizeing width or resizing height
int resizeType = 0;


//	open a file
char* openFile() {
	char szFileName[MAX_PATH] = {0};
	OPENFILENAME openFileName = {0};
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.nMaxFile = MAX_PATH;
	openFileName.lpstrFilter = "图片文件(*.png;*.jpg;*.bmp)\0*.png;*.jpg;*.bmp\0所有文件(*.*)\0*.*\0\0";
	openFileName.lpstrFile = szFileName;
	openFileName.nFilterIndex = 1;
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (!::GetOpenFileName(&openFileName)) return NULL;
	char *fileName = new char[999];
	strcpy(fileName, openFileName.lpstrFile);
	return fileName;
}

//	create window
void createWindow(const char *filename) {
	cvNamedWindow(windowName);

	IplImage *imageLoad = cvLoadImage(filename);

	//	maximum window size is 1.5 times as initial image 
	maxSize.width = (nowSize.width = imageLoad->width) * 1.5;
	maxSize.height = (nowSize.height = imageLoad->height) * 1.5;

	image = cvCreateImage(maxSize, IPL_DEPTH_8U, 3);
	cvSet(image, CV_RGB(0, 0, 0));

	for (int x = 0; x < imageLoad->width; ++x)
		for (int y = 0; y < imageLoad->height; ++y)
			cvSet2D(image, y, x, cvGet2D(imageLoad, y, x));

	cvReleaseImage(&imageLoad);
	cvShowImage(windowName, image);
}
void createWindow(const char *filename, int sp) {
	cvNamedWindow(windowName);
	image = cvLoadImage(filename);
	cvShowImage(windowName, image);
}

//	destroy window
void destroyWindow() {
	cvDestroyWindow(windowName);
	cvReleaseImage(&image);
}

//	mouse event function
void mouseEvent(int mevent, int posx, int posy, int flags, void *ustc) {

	//	detect if boundary is choosen if left button down
	if (mevent == CV_EVENT_LBUTTONDOWN) {
		if (abs(posx - nowSize.width) < RESIZE_DETECT_SIZE) {
			resizeType = 1;
		}
		if (abs(posy - nowSize.height) < RESIZE_DETECT_SIZE) {
			resizeType = 2;
		}
	}

	//	resize width or height if left up 
	if (mevent == CV_EVENT_LBUTTONUP) {
		//CostFlow *method = new CostFlow(image, nowSize);
		DynamicP *method = new DynamicP(image, nowSize);

		//	resize width
		if (resizeType == 1) {
			printf("Begin resizing X...\n");
			method->resizeWidth(posx);
			printf("Complete!\n\n");
			nowSize.width = posx;
			cvShowImage(windowName, image);
			::MessageBoxA(NULL, "Complete!", "OK", 0x00000040);
		}

		//	resize height
		if (resizeType == 2) {
			printf("Begin resizing Y..\n");
			method->resizeHeight(posy);
			printf("Complete!\n\n");
			nowSize.height = posy;
			cvShowImage(windowName, image);
			::MessageBoxA(NULL, "Complete!", "OK", 0x00000040);
		}

		resizeType = 0;
	}
}

int main() {

	//	choose file
	::MessageBoxA(NULL, "Choose the image!", "Begin", 0x00000040);
	char *fileName = openFile();
	if (fileName == NULL) return 0;

	createWindow(fileName);
	cvSetMouseCallback(windowName, mouseEvent);
	cvWaitKey();
	destroyWindow();
	
	/*
	createWindow(fileName, 0);
	ShortestPath *shortestPath = new ShortestPath(image);
	while (true) {
		int key = cvWaitKey(20);
		if (key == 'a') {
			shortestPath->seamCarvingW();
			cvShowImage(windowName, image);
		}
	}
	*/

	return 0;
}