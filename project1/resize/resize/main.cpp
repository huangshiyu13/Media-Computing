#include <ctime>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include "FWH.h"
#include "basic.h"
#include "costflow.h"
#include "dynamicp.h"
#include "shortestpath.h"

//去除命令行窗口
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


#define RESIZE_DETECT_SIZE 10
const char *winTitle = "Resize";
IplImage *image;
CvSize winSize, imgSize;

int resizeType = 0;


//打开一张图片
char* openFile() {
	//MAX_PATH = 260
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

void createWindow(const char *filename) {
	cvNamedWindow(winTitle);

	IplImage *imageLoad = cvLoadImage(filename);

	winSize.width = (imgSize.width = imageLoad->width) * 1.5;
	winSize.height = (imgSize.height = imageLoad->height) * 1.5;

	image = cvCreateImage(winSize, IPL_DEPTH_8U, 3);
	cvSet(image, CV_RGB(1, 36, 86));

	for (int x = 0; x < imageLoad->width; ++x)
		for (int y = 0; y < imageLoad->height; ++y)
			cvSet2D(image, y, x, cvGet2D(imageLoad, y, x));

	cvReleaseImage(&imageLoad);
	cvShowImage(winTitle, image);

}
void createWindow(const char *filename, int sp) {
	cvNamedWindow(winTitle);
	image = cvLoadImage(filename);
	cvShowImage(winTitle, image);
}

void destroyWindow() {
	cvDestroyWindow(winTitle);
	cvReleaseImage(&image);
}

void onMouse(int Event, int x, int y, int flags, void *param ) {

	if (Event == CV_EVENT_LBUTTONDOWN) {
		if (abs(x - imgSize.width) < RESIZE_DETECT_SIZE) {
			resizeType = 1;
		}
		if (abs(y - imgSize.height) < RESIZE_DETECT_SIZE) {
			resizeType = 2;
		}
	}

	//	resize width or height if left up 
	if (Event == CV_EVENT_LBUTTONUP && resizeType != 0) {
		//CostFlow *method = new CostFlow(image, imgSize);
		DynamicP *method = new DynamicP(image, imgSize);

		if (resizeType == 1) {
			method->resizeWidth(x);
			imgSize.width = x;
			cvShowImage(winTitle, image);
			::MessageBoxA(NULL, "完成", "提示", 0x00000040);
		}

		//	resize height
		if (resizeType == 2) {
			printf("Begin resizing Y..\n");
			method->resizeHeight(y);
			printf("Complete!\n\n");
			imgSize.height = y;
			cvShowImage(winTitle, image);
			::MessageBoxA(NULL, "完成", "提示", 0x00000040);
		}

		resizeType = 0;
	}
}

int main() {
	if (::MessageBoxA(NULL, "请选择一张图片", "提醒", 0x00000001L) == 1){
		char *fileName;
		if ( (fileName = openFile()) == NULL) return 0;

		createWindow(fileName);
		cvSetMouseCallback(winTitle, onMouse);
		while((cvWaitKey(10)&0xff) != 27);//ESC
		destroyWindow();
	}
	return 0;
}