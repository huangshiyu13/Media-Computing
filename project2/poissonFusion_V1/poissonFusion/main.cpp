#include <vector>

#include "BakImage.h"
#include "tool.h"

using namespace std;

int main() {

	//对运行模式进行选择
	//selectModel();

	if (::MessageBoxA(NULL, "请选择一张被截取的图片", "提醒", 0x00000001L) == 1){
		char *cutImageFile = openFile();
		if (cutImageFile == NULL) return 0;

		if (::MessageBoxA(NULL, "请选择一张背景图片", "提醒", 0x00000001L) != 1){
			return 0;
		}
		char *bakImageFile = openFile();
		if (bakImageFile == NULL) return 0;

		cutImage = new CutImage(cutImageFile, "cutImageWindow");
		bakImage = new BakImage(bakImageFile, "backgroundImageWindow");
		
		//初始化
		cutImage->init();

		//显示窗口
		cutImage->createWindow();
		bakImage->createWindow();

		//监听鼠标事件
		cvSetMouseCallback(cutImage->winName.c_str(), cutImageMouseEvent, NULL);
		cvSetMouseCallback(bakImage->winName.c_str(), bakImageMouseEvent, NULL);

		while(cvWaitKey(10) != 27);//按ESC退出
	}
	return 0;
}

