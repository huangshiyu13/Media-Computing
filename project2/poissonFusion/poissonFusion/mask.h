#ifndef MASK_H
#define MASK_H

#include "define.h"

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
#endif