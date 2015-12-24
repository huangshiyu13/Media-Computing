#ifndef CUTIMAGE_H
#define CUTIMAGE_H


#include <string>
#include "define.h"
using namespace std;

bool inFirstRange = true;
int closeRange = 10;
class Mask {
public:
	bool **mask;
	int cutImgW, cutImgH;
	int minx, miny, maxx, maxy,cx, cy;
	int size;

	Mask(int _cutImgW, int _cutImgH) {
		cutImgW = _cutImgW;
		cutImgH = _cutImgH;
		mask = new bool*[cutImgW];
		for (int x = 0; x < cutImgW; ++x) {
			mask[x] = new bool[cutImgH];
			memset(mask[x], false, cutImgH * sizeof(bool));
		}
	}
	~Mask() {
		for (int x = 0; x < cutImgW; ++x) delete[] mask[x];
		delete[] mask;
	}

	bool get(int x, int y) {
		if (x < 0 || x >= cutImgW) return false;
		if (y < 0 || y >= cutImgH) return false;
		return mask[x][y];
	}

	void set(int x, int y, bool b) {
		if (x < 0 || x >= cutImgW) return;
		if (y < 0 || y >= cutImgH) return;
		mask[x][y] = b;
	}

	//	get mask information
	void getSizeInfo() {
		minx = miny = 1<<30;
		maxx = maxy = -1<<30;
		size = 0;
		for (int x = 0; x < cutImgW; ++x){
			for (int y = 0; y < cutImgH; ++y){
				if (mask[x][y]) {
					minx = min(minx, x);
					miny = min(miny, y);
					maxx = max(maxx, x);
					maxy = max(maxy, y);
					size++;
				}
			}
		}
		cx = (minx + maxx) / 2;
		cy = (miny + maxy) / 2;
	}
};

Mask *msk;
class CutImage {
public:
	string winName;
	IplImage *oriImage;
	IplImage *image;

	int width, height;
	CvSize size;

	CutImage(const char *filename, string winName){
		image = cvLoadImage(filename);
		oriImage = cvCloneImage(image);
		width = image->width;
		height = image->height;
		size = cvSize(width,height);
		this->winName = winName;

	}

	~CutImage() {
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

	void init(){
		msk = new Mask(width, height);

		id = new int*[width];

		c = new CvScalar*[width];
		visit = new bool*[width];
		for (int x = 0; x < width; ++x) {
			id[x] = new int[height];
			memset(id[x], 0, height * sizeof(int));
			c[x] = new CvScalar[height];
			for (int y = 0; y < height; ++y) {
				c[x][y] = INVCOLOR(getColor(x, y));
			}
			visit[x] = new bool[height];
			memset(visit[x], 0, height * sizeof(bool));
		}

		visitQue = new pair<int,int>[width * height];
	}
};

CutImage *cutImage;

//	draw a line based on two end points
void ringDrawLine(int lx, int ly, int nx, int ny) {

	//	out
	if (lx == -1) return;

	//	just on point
	if (lx == nx && ly == ny) {
		cutImage->setColor(lx, ly, c[lx][ly]);
		return;
	}

	//	compare length of directions of x-axis and y-axis and enumerate the larger one
	if (abs(lx - nx) < abs(ly - ny)) {
		double dx = 1.0 * (nx - lx) / abs(ny - ly);
		int dy = ly < ny ? 1 : -1;
		for (int cnt = 0; ly != ny + dy; ly += dy, cnt++) {
			int tx = (int)(lx + dx * cnt + 0.5);
			cutImage->setColor(tx, ly, c[tx][ly]);
			ringPath.push_back(make_pair(tx, ly));
		}
	} else {
		double dy = 1.0 * (ny - ly) / abs(nx - lx);
		int dx = lx < nx ? 1 : -1;
		for (int cnt = 0; lx != nx + dx; lx += dx, cnt++) {
			int ty = (int)(ly + dy * cnt + 0.5);
			cutImage->setColor(lx, ty, c[lx][ty]);
			ringPath.push_back(make_pair(lx, ty));
		}
	}
}

void reDrawPath(){
	for (int i = 0; i < ringPath.size(); ++i) {
		int x = ringPath[i].first;
		int y = ringPath[i].second;
		cutImage->setColor(x, y, c[x][y]);
	}
}

void clearPath(){
	//	restore the color of ring in cutImage image
	for (int i = 0; i < ringPath.size(); ++i) {
		int x = ringPath[i].first;
		int y = ringPath[i].second;
		cutImage->setColor(x, y, INVCOLOR(c[x][y]));
	}

	//	restore variables
	ringed = false;
	ringLastX = ringLastY = -1;
	ringPath.clear();
	cutImage->show();
}

int distance2(int pointX, int pointY, int targetX, int targetY){
	return (pointX-targetX)*(pointX-targetX) + (pointY-targetY)*(pointY-targetY);
}

bool inRange(int pointX, int pointY, int targetX, int targetY , int range){
	return distance2(pointX,pointY,targetX,targetY) <= range*range?true:false;
}

bool isRange(int pointX, int pointY, int targetX, int targetY , int range){
	return distance2(pointX,pointY,targetX,targetY) == range*range?true:false;
}

void drawPoint(int x, int y , int range){
	for (int i = x-range ; i <= x+range ; i++){
		for (int j = y-range ; j <= y+range ; j++){
			if (abs(i-x)==range||abs(j-y) == range){
				cutImage->setColor(i, j, CV_RGB(255,0,0));
			}
		}
	}
}

void clearPoint(int x, int y , int range){
	for (int i = x-range ; i <= x+range ; i++){
		for (int j = y-range ; j <= y+range ;j++){
			if (abs(i-x)==range||abs(j-y) == range){
				cutImage->setColor(i, j,INVCOLOR(c[i][j]));
			}
		}
	}
}


void cutImageMouseEvent(int mevent, int posx, int posy, int flags, void *ustc) {

	//	exit if poisson image editing is finished
	if (poissonImageEditinged) return;

	//	cancel the ring if right button down
	if (mevent == CV_EVENT_RBUTTONDOWN) {
		clearPath();
		return;
	}

	if (!ringed && mevent == CV_EVENT_LBUTTONDOWN) {
		ringStartX = posx;
		ringStartY = posy;
		inFirstRange = true;
	}

	if (!ringed && flags == CV_EVENT_FLAG_LBUTTON) {
		ringDrawLine(ringLastX, ringLastY, posx, posy);
		ringLastX = posx;
		ringLastY = posy;

		if (inRange(posx, posy, ringStartX, ringStartY,closeRange) && !inFirstRange){
			drawPoint(ringStartX,ringStartY, closeRange);
		}
		else{
			if (!inRange(posx, posy, ringStartX, ringStartY,3*closeRange)) inFirstRange = false;
			clearPoint(ringStartX,ringStartY, closeRange);
			reDrawPath();
		}		
		cutImage->show();
	}

	if (!ringed && mevent == CV_EVENT_LBUTTONUP ) {
		clearPoint(ringStartX,ringStartY, closeRange);
		if (inRange(posx, posy, ringStartX, ringStartY,closeRange) && !inFirstRange){
			ringDrawLine(posx, posy, ringStartX, ringStartY);
			reDrawPath();
			ringed = true;
			cutImage->show();
		}
		else{
			clearPath();
		}
	}
}

#endif