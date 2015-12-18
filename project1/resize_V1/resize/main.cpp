#include <ctime>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include "FWH.h"
#include "basic.h"
#include "preProcess.h"

//去除命令行窗口
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


#define RESIZE_DETECT_SIZE 10
const char *winTitle = "Resize";
IplImage *image,*oriImg;
FWH** fwh;
CvSize winSize, imgSize,oriSize,minSize;

int resizeType = 0;


void cutW(CutPath path , CvSize size){
	//cout << "inW1"<<endl;
	int H = size.height;
	int W = size.width;
	//cout << "inW2"<<endl;
	//cout << "inW3"<<endl;
	for (int y = H - 1; y >= 0; --y) {
		//cout << y << endl;
		for (int x = path[H-1-y]; x < W - 1; ++x) {
			cvSet2D(image, y, x, cvGet2D(image,y,x+1));
		}
		cvSet2D(image, y, W-1, CV_RGB(222, 222, 222));
	}
	//cout << "inW4"<<endl;
}

void cutH(CutPath path , CvSize size){
	//cout << "inH1"<<endl;
	int H = size.height;
	int W = size.width;
	//cout << "inH2"<<endl;
	
	//cout << "inH3"<<endl;
	for (int x = W - 1; x >= 0; --x) {
		//cout << x << " "<<path.size() << endl;
		for (int y = path[W-1-x]; y < H - 1; ++y) {
			//cout << x << " " << y <<endl;
			cvSet2D(image, y, x, cvGet2D(image,y+1,x));
		}
		
		cvSet2D(image, H-1, x, CV_RGB(222, 222, 222));
		
	}
	//cout << "inH4"<<endl;
}

void getImage(CvSize size){
	//cout << size.height << " " << size.width << endl;
	if (size.height == oriSize.height && size.width == oriSize.width){
		image = cvCloneImage(oriImg);
		return;
	}

	int W = fwh[size.height][size.width].WPre;
	int H = fwh[size.height][size.width].HPre;
	getImage(cvSize(W,H));
	//cout <<"out "<< size.height << " " << size.width << endl;
	switch(fwh[size.height][size.width].type){
	case ReduceW:
		//cout << "W" << endl;
		cutW(fwh[size.height][size.width].cutPath , cvSize(W,H));
		break;
	case ReduceH:
		//cout << "H" << endl;
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
	winSize.width = (imgSize.width = imageLoad->width)*2;
	winSize.height = (imgSize.height = imageLoad->height)*2;
	oriSize.width = imgSize.width;
	oriSize.height = imgSize.height;
	image = cvCreateImage(winSize, IPL_DEPTH_8U, 3);
	cvSet(image, CV_RGB(222, 222, 222));

	for (int x = 0; x < imageLoad->width; ++x)
		for (int y = 0; y < imageLoad->height; ++y)
			cvSet2D(image, y, x, cvGet2D(imageLoad, y, x));

	minSize = cvSize(int(imgSize.width*0.8),int(imgSize.height*0.8));
	fwh = new FWH*[imgSize.height+1];
	for (int i = 0 ; i < imgSize.height+1; i++){
		fwh[i] = new FWH[imgSize.width+1];
	}
	IplImage* copyImg = cvCloneImage(image);
	oriImg = cvCloneImage(image);
	preProcess(fwh, copyImg , imgSize , minSize, winSize);

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
		if (abs(x - imgSize.width) < RESIZE_DETECT_SIZE && abs(y - imgSize.height) < RESIZE_DETECT_SIZE){
			resizeType = 3;
		}
	}
	if (Event == CV_EVENT_MOUSEMOVE && resizeType != 0){
		if (resizeType == 1) {
			//method->resizeWidth(x);
			imgSize.width = x>oriSize.width?oriSize.width:x;
			imgSize.width = imgSize.width<minSize.width?minSize.width:imgSize.width;
			//cout <<"change width:"<< imgSize.height << " "<<imgSize.width<<endl;
			//cvShowImage(winTitle, image);
			
		}

		//	resize height
		if (resizeType == 2) {
			//method->resizeHeight(y);
			imgSize.height = y>oriSize.height?oriSize.height:y;
			imgSize.height = imgSize.height<minSize.height?minSize.height:imgSize.height;
			//cout <<"change height:"<< imgSize.height << " "<<imgSize.width<<endl;
			//cvShowImage(winTitle, image);
			
		}
		if (resizeType == 3){
			//method->resizeHeight(y);
			imgSize.width = x>oriSize.width?oriSize.width:x;
			imgSize.width = imgSize.width<minSize.width?minSize.width:imgSize.width;
			imgSize.height = y>oriSize.height?oriSize.height:y;
			imgSize.height = imgSize.height<minSize.height?minSize.height:imgSize.height;
			//cout <<"change point:"<< imgSize.height << " "<<imgSize.width<<endl;
			//cvShowImage(winTitle, image);	
		}
		getImage(imgSize);
		cvShowImage(winTitle, image);
	}
	//	resize width or height if left up 
	if (Event == CV_EVENT_LBUTTONUP && resizeType != 0) {
		cout << imgSize.height << " "<<imgSize.width<<endl;
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