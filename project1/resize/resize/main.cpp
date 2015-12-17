#include <ctime>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include "FWH.h"
#include "basic.h"
#include "costflow.h"
#include "dynamicp.h"
#include "shortestpath.h"
#include "preProcess.h"

//去除命令行窗口
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


#define RESIZE_DETECT_SIZE 10
const char *winTitle = "Resize";
IplImage *image,*oriImg;
FWH** fwh;
CvSize winSize, imgSize;

int resizeType = 0;


void cutW(CutPath path , CvSize size){
	int H = size.height;
	int W = size.width;
	for (int x = 0; x < W; ++x)
		for (int y = 0; y < H; ++y){
			//cout << x << " " << y << endl;
			myCV2D[x][y] = cvGet2D(image, y, x);
		}

	for (int y = H - 1; y >= 0; --y) {
		for (int x = path[y].width; x < W - 1; ++x) {
			cvSet2D(image, y, x, myCV2D[x + 1][y]);
		}
	}
}

void cutH(CutPath path , CvSize size){
	int H = size.height;
	int W = size.width;
	for (int x = 0; x < W; ++x)
		for (int y = 0; y < H; ++y){
			//cout << x << " " << y << endl;
			myCV2D[x][y] = cvGet2D(image, y, x);
		}

	for (int x = W - 1; x >= 0; --x) {
		for (int y = path[x].height; y < H - 1; ++y) {
			cvSet2D(image, y, x, myCV2D[x ][y+1]);
		}
	}
}


void getImage(CvSize size){
	cout << size.width << " " << size.height << endl;
	if (size.height == winSize.height && size.width == winSize.width){
		
		image = cvCloneImage(oriImg);
	
		return;
	}
	int W = fwh[size.height][size.width].WPre;
	int H = fwh[size.height][size.width].HPre;
	getImage(cvSize(H,W));
	cout <<"out "<< size.width << " " << size.height << endl;
	switch(fwh[size.height][size.width].type){
	case ReduceW:
		cutW(fwh[size.height][size.width].cutPath , cvSize(W,H));
		break;
	case ReduceH:
		cutH(fwh[size.height][size.width].cutPath , cvSize(W,H));
		break;
	case IncreaseW:
		break;
	case IncreaseH:
		break;

	}


}

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
	IplImage *imageLoad = cvLoadImage(filename);
	winSize.width = (imgSize.width = imageLoad->width);
	winSize.height = (imgSize.height = imageLoad->height);

	image = cvCreateImage(imgSize, IPL_DEPTH_8U, 3);
	cvSet(image, CV_RGB(1, 36, 86));

	for (int x = 0; x < imageLoad->width; ++x)
		for (int y = 0; y < imageLoad->height; ++y)
			cvSet2D(image, y, x, cvGet2D(imageLoad, y, x));

	CvSize minSize = cvSize(int(imgSize.width*0.5),int(imgSize.height*0.5));
	fwh = new FWH*[imgSize.height+1];
	for (int i = 0 ; i < imgSize.height+1; i++){
		fwh[i] = new FWH[imgSize.width+1];
	}
	IplImage* copyImg = cvCloneImage(image);
	oriImg = cvCloneImage(image);
	preProcess(fwh, copyImg , imgSize , minSize, imgSize);

	cvNamedWindow(winTitle);
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
		//DynamicP *method = new DynamicP(image, imgSize);

		if (resizeType == 1) {
			//method->resizeWidth(x);
			imgSize.width = x;
			cout <<"change width:"<< imgSize.height << " "<<imgSize.width<<endl;
			//cvShowImage(winTitle, image);
			getImage(imgSize);
			cvShowImage(winTitle, image);
		}

		//	resize height
		if (resizeType == 2) {
			//method->resizeHeight(y);
			imgSize.height = y;
			cout <<"change height:"<< imgSize.height << " "<<imgSize.width<<endl;
			//cvShowImage(winTitle, image);
			getImage(imgSize);
			cvShowImage(winTitle, image);
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