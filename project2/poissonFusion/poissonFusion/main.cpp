#include <vector>

#include "BakImage.h"
#include "tool.h"

using namespace std;

int main() {
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

		msk = new Mask(cutImage->width, cutImage->height);

		id = new int*[cutImage->width];

		c = new CvScalar*[cutImage->width];
		visit = new bool*[cutImage->width];
		for (int x = 0; x < cutImage->width; ++x) {
			id[x] = new int[cutImage->height];
			memset(id[x], 0, cutImage->height * sizeof(int));
			c[x] = new CvScalar[cutImage->height];
			for (int y = 0; y < cutImage->height; ++y) {
				c[x][y] = INVCOLOR(cutImage->getColor(x, y));
			}
			visit[x] = new bool[cutImage->height];
			memset(visit[x], 0, cutImage->height * sizeof(bool));
		}

		visitQue = new pair<int,int>[cutImage->width * cutImage->height];

		cutImage->createWindow();
		bakImage->createWindow();

		cvSetMouseCallback(cutImage->winName.c_str(), cutImageMouseEvent, NULL);
		cvSetMouseCallback(bakImage->winName.c_str(), bakImageMouseEvent, NULL);

		while(cvWaitKey(10) != 27);//ESC
	}
	return 0;
}