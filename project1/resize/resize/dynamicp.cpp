#include <algorithm>
#include "dynamicp.h"
using namespace std;


#define DXN 3
#define DYN 3

const int DX[] = {-1, 0, 1};
const int DY[] = {-1, 0, 1};

CvScalar cv2D[1200][900];


DynamicP::DynamicP(IplImage *_image, CvSize size) {
	image = _image;
	width = size.width;
	height = size.height;
}

DynamicP::~DynamicP() {
	for (int x = 0; x < width; ++x) {
		delete[] f[x];
		delete[] pre[x];
	}
	delete[] f;
	delete[] pre;
}

void DynamicP::resizeWidth(int newWidth) {

	//	preprocess
	int maxWidth = max(width, newWidth);
	f = new double*[maxWidth];
	pre = new int*[maxWidth];
	ban = new bool**[maxWidth];
	for (int x = 0; x < maxWidth; ++x) {
		f[x] = new double[height];
		pre[x] = new int[height];
		ban[x] = new bool*[height];
		for (int y = 0; y < height; ++y) {
			ban[x][y] = new bool[DXN];
			for (int d = 0; d < DXN; ++d) ban[x][y][d] = false;
		}
	}
	
	//	extract best path each iteration
	int maxTimes = abs(newWidth - width);
	for (int times = 0; times < maxTimes; ++times) {
		for (int x = 0; x < width; ++x)
			for (int y = 0; y < height; ++y)
				cv2D[x][y] = cvGet2D(image, y, x);

		//	DP
		for (int x = 0; x < width; ++x) f[x][0] = 0;

		for (int y = 1; y < height; ++y)
			for (int x = 0; x < width; ++x) {
				f[x][y] = 1e20;
				for (int d = 0; d < DXN; ++d) {
					int xx = x + DX[d];
					if (xx < 0 || xx >= width) continue;
					if (ban[x][y][d]) continue;
					double now = f[xx][y - 1] + dist(cv2D[x][y], cv2D[xx][y - 1]);
					if (now < f[x][y]) {
						f[x][y] = now;
						pre[x][y] = d;
					}
				}
			}

		int bestx = 0;
		for (int x = 0; x < width; ++x) {
			bestx = f[bestx][height - 1] < f[x][height - 1] ? bestx : x;
		}

		//	shrink or expand
		if (newWidth < width) {
			for (int y = height - 1; y >= 0; --y) {
				for (int x = bestx; x < width - 1; ++x) {
					cvSet2D(image, y, x, cv2D[x + 1][y]);
				}
				cvSet2D(image, y, width - 1, CV_RGB(0, 0, 0));
				if (y == 0) break;
				int d = pre[bestx][y];
				bestx += DX[d];
			}
			width--;
		} else {
			for (int y = height - 1; y >= 0; --y) {
				for (int x = width; x > bestx; --x) {
					cvSet2D(image, y, x, cv2D[x - 1][y]);
					for (int d = 0; d < DXN; ++d) ban[x][y][d] = ban[x - 1][y][d];
				}
				if (y == 0) break;
				int d = pre[bestx][y];
				ban[bestx][y][d] = ban[bestx + 1][y][d] = true;
				bestx += DX[d];
			}
			width++;
		}
	}
}

void DynamicP::resizeHeight(int newHeight) {
	int maxHeight = max(height, newHeight);
	f = new double*[width];
	pre = new int*[width];
	ban = new bool**[width];
	for (int x = 0; x < width; ++x) {
		f[x] = new double[maxHeight];
		pre[x] = new int[maxHeight];
		ban[x] = new bool*[maxHeight];
		for (int y = 0; y < maxHeight; ++y) {
			ban[x][y] = new bool[DYN];
			for (int d = 0; d < DYN; ++d) ban[x][y][d] = false;
		}
	}

	int maxTimes = abs(newHeight - height);
	for (int times = 0; times < maxTimes; ++times) {
		for (int x = 0; x < width; ++x)
			for (int y = 0; y < height; ++y)
				cv2D[x][y] = cvGet2D(image, y, x);
		
		for (int y = 0; y < height; ++y) f[0][y] = 0;

		for (int x = 1; x < width; ++x)
			for (int y = 0; y < height; ++y) {
				f[x][y] = 1e20;
				for (int d = 0; d < DYN; ++d) {
					int yy = y + DY[d];
					if (yy < 0 || yy >= height) continue;
					if (ban[x][y][d]) continue;
					double now = f[x - 1][yy] + dist(cv2D[x][y], cv2D[x - 1][yy]);
					if (now < f[x][y]) {
						f[x][y] = now;
						pre[x][y] = d;
					}
				}
			}

		int besty = 0;
		for (int y = 0; y < height; ++y) {
			besty = f[width - 1][besty] < f[width - 1][y] ? besty : y;
		}

		if (newHeight < height) {
			for (int x = width - 1; x >= 0; --x) {
				for (int y = besty; y < height - 1; ++y) {
					cvSet2D(image, y, x, cv2D[x][y + 1]);
				}
				cvSet2D(image, height - 1, x, CV_RGB(0, 0, 0));
				if (x == 0) break;
				int d = pre[x][besty];
				besty += DY[d];
			}
			height--;
		} else {
			for (int x = width - 1; x >= 0; --x) {
				for (int y = height; y > besty; --y) {
					cvSet2D(image, y, x, cv2D[x][y - 1]);
					for (int d = 0; d < DYN; ++d) ban[x][y][d] = ban[x][y - 1][d];
				}
				if (x == 0) break;
				int d = pre[x][besty];
				ban[x][besty][d] = ban[x][besty + 1][d] = true;
				besty += DY[d];
			}
			height++;
		}
	}
}