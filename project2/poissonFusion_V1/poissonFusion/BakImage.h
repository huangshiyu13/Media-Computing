#ifndef BAKIMAGE_H
#define BAKIMAGE_H


#include <string>
#include "define.h"
#include "CutImage.h"
#include <windows.h>
#include "GaussSeidelSolver.h"

using namespace std;

class GaussSeidelSolver;

class BakImage {
public:
	string winName;
	IplImage *oriImage;
	IplImage *image;
	
	int width, height;
	CvSize size;

	BakImage(const char *filename, string winName){
		image = cvLoadImage(filename);
		oriImage = cvCloneImage(image);
		width = image->width;
		height = image->height;
		size = cvSize(width,height);
		this->winName = winName;
		
	}

	~BakImage() {
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
BakImage *bakImage;


void poissonImageEditing(int posx, int posy) {
	msk->getSizeInfo();

	for (int k = 0; k < 3; ++k) {

		GaussSeidelSolver gaussSeidelSolver(msk->size);

		int idCnt = 0;
		for (int x = 0; x < cutImage->width; ++x)
			for (int y = 0; y < cutImage->height; ++y)
				if (msk->get(x, y))
					id[x][y] = idCnt++;

		for (int x = 0; x < cutImage->width; ++x){
			for (int y = 0; y < cutImage->height; ++y){
				if (msk->get(x, y)) {

					//	new an equation
					gaussSeidelSolver.addEquation(id[x][y]);

					//	add GSIUnit of now pixel
					gaussSeidelSolver.addGSIUnit(id[x][y], 4);

					double b = 0;

		
					for (int d = 0; d < dn; ++d) {

						//	coordinate of adjacent pixel
						int xx = x + dx[d];
						int yy = y + dy[d];

						if (msk->get(xx, yy) ) {

							//	add GSIUnit if adjacent pixel is in editing region
							gaussSeidelSolver.addGSIUnit(id[xx][yy], -1);

							//	calculate right considering blending
#ifdef BLENDING_GRADIENT
							b += ( cutImage->getColor(x, y, k) - cutImage->getColor(xx, yy, k) ) * SOURCE_RATIO;
							b += ( bakImage->getColor(TARX(x), TARY(y), k) - bakImage->getColor(TARX(xx), TARY(yy), k) ) * (1 - SOURCE_RATIO);
#else
							b += cutImage->getColor(x, y, k) - cutImage->getColor(xx, yy, k);
#endif
						} else {
							//	calculate right if adjacent pixel is not in editing region
							b += bakImage->getColor(TARX(x), TARY(y), k);
						}
					}

					//	add right of now equation
					gaussSeidelSolver.addB(b);
				}
			}
		}

		// solve equations
		gaussSeidelSolver.solve(0.1, 0);

		//	get colors of pixels in editing region
		for (int x = 0; x < cutImage->width; ++x)
			for (int y = 0; y < cutImage->height; ++y)
				if (msk->get(x, y))
					c[x][y].val[k] = gaussSeidelSolver.ans[id[x][y]];
	}

	//	renew bakImage image
	for (int x = 0; x < cutImage->width; ++x)
		for (int y = 0; y < cutImage->height; ++y)
			if (msk->get(x, y)) {
				CvScalar newColor;
				for (int k = 0; k < 3; ++k) newColor.val[k] = c[x][y].val[k];
#ifndef BLENDING_GRADIENT
				newColor = newColor * SOURCE_RATIO + bakImage->getColor(TARX(x), TARY(y)) * (1 - SOURCE_RATIO);
#endif
				bakImage->setColor(TARX(x), TARY(y), newColor);
			}
}

//	use BFS to find the connected block
void visitBFS(int sx, int sy) {
	int tail = 0;
	visitQue[tail] = make_pair(sx, sy);
	for (int head = 0; head <= tail; ++head) {
		int x = visitQue[head].first;
		int y = visitQue[head].second;
		for (int d = 0; d < dn; ++d) {
			int xx = x + dx[d];
			int yy = y + dy[d];
			if (xx < 0 || xx >= cutImage->width || yy < 0 || yy >= cutImage->height) {
				visitOut = true;
				continue;
			}
			if (msk->get(xx, yy) || visit[xx][yy]) continue;
			visit[xx][yy] = true;
			visitRegion.push_back(make_pair(xx, yy));
			visitQue[++tail] = make_pair(xx, yy);
		}
	}
}


void bakImageMouseEvent(int mevent, int posx, int posy, int flags, void *ustc) {

	//当生成了圆圈且按钮按下时，执行这里
	if (ringed && mevent == CV_EVENT_LBUTTONDOWN) {

		//判断截图区是否查出了背景图片的边界
		int minx = 1<<30, miny = 1<<30;
		int maxx = -1<<30, maxy = -1<<30;
		for (int i = 0; i < ringPath.size(); ++i) {
			int x = ringPath[i].first;
			int y = ringPath[i].second;
			minx = min(minx, x);
			miny = min(miny, y);
			maxx = max(maxx, x);
			maxy = max(maxy, y);
		}
		int cx = (minx + maxx) / 2;
		int cy = (miny + maxy) / 2;

		if (posx - cx + minx <= 0 || posx - cx + maxx >= bakImage->width - 1 || posy - cy + miny <= 0 || posy - cy + maxy >= bakImage->height - 1) {
			::MessageBoxA(NULL, "截图范围超出边界!", "Error", 0x00000030);
			return;
		}

		//设置遮罩的边界
		for (int i = 0; i < ringPath.size(); ++i) {
			msk->set(ringPath[i].first, ringPath[i].second, true);
		}

		//标记出所有在圈内的像素
		for (int x = 0; x < cutImage->width; ++x){
			for (int y = 0; y < cutImage->height; ++y) {
				if (!msk->get(x, y) && !visit[x][y]) {
					visitOut = false;
					visitRegion.clear();
					visitBFS(x, y);
					if (!visitOut) {
						for (int i = 0; i < visitRegion.size(); ++i) {
							msk->set(visitRegion[i].first, visitRegion[i].second, true);
						}
					}
				}
			}
		}

		//	restore the color of cutImage image
		for (int i = 0; i < ringPath.size(); ++i) {
			int x = ringPath[i].first;
			int y = ringPath[i].second;
			cutImage->setColor(x, y, INVCOLOR(c[x][y]));
		}

		//	poisson image editing
		poissonImageEditing(posx, posy);
		poissonImageEditinged = true;

		//	modify the color of editing region in cutImage image
		/*for (int x = 0; x < cutImage->width; ++x)
		for (int y = 0; y < cutImage->height; ++y)
		if (msk->get(x, y)) {
		cutImage->setColor(x, y, cutImage->getColor(x, y) * 0.5 + CV_RGB(255, 255, 255) * 0.5);
		}*/

		cutImage->show();
		bakImage->show();
		::MessageBoxA(NULL, "Poisson image editing completed!", "OK", 0x00000040);
	}
}

#endif