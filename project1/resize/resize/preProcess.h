#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "FWH.h"
#define DXN 3
#define DYN 3
const int DX[] = {-1, 0, 1};
const int DY[] = {-1, 0, 1};

int costDW(IplImage Img, int W , int H, CutPath &cutPath, IpImage ImgR){
	double f[W][H];
	int pre[W][H];

	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
			cv2D[x][y] = cvGet2D(Img, y, x);

	for (int x = 0; x < W; ++x) f[x][0] = 0;

	for (int y = 1; y < H; ++y)
		for (int x = 0; x < W; ++x) {
			f[x][y] = 1e20;
			for (int d = 0; d < DXN; ++d) {
				int xx = x + DX[d];
				if (xx < 0 || xx >= W) continue;
				double now = f[xx][y - 1] + dist(cv2D[x][y], cv2D[xx][y - 1]);
				if (now < f[x][y]) {
					f[x][y] = now;
					pre[x][y] = d;
				}
			}
		}

		int bestx = 0;
		int minCost = 0 ;
		for (int x = 0; x < W; ++x) {
			bestx = f[bestx][height - 1] < f[x][height - 1] ? bestx : x;
			minCost = f[bestx][height - 1];
		}


		for (int y = height - 1; y >= 0; --y) {
			cutPath.push_back(bestx,y);
			if (y == 0) break;
			int d = pre[bestx][y];
			bestx += DX[d];
		}
		return minCost;
}

int costDH(IplImage Img, int W , int H, CutPath &cutPath){
	double f[W][H];
	int pre[W][H];

	for (int x = 0; x < W; ++x)
		for (int y = 0; y < H; ++y)
			cv2D[x][y] = cvGet2D(Img, y, x);


	for (int x = 0; x < W; ++x)
		for (int y = 0; y < H; ++y)
			cv2D[x][y] = cvGet2D(Img, y, x);

	for (int y = 0; y < H; ++y) f[0][y] = 0;

	for (int x = 1; x < W; ++x)
		for (int y = 0; y < H; ++y) {
			f[x][y] = 1e20;
			for (int d = 0; d < DYN; ++d) {
				int yy = y + DY[d];
				if (yy < 0 || yy >= height) continue;
				double now = f[x - 1][yy] + dist(cv2D[x][y], cv2D[x - 1][yy]);
				if (now < f[x][y]) {
					f[x][y] = now;
					pre[x][y] = d;
				}
			}
		}

	int besty = 0;
	int minCost = 0;
	for (int y = 0; y < H; ++y) {
		besty = f[W - 1][besty] < f[W - 1][y] ? besty : y;
		minCost = f[W - 1][besty];
	}

	for (int x = width - 1; x >= 0; --x) {
		cutPath.push_back(x,besty);
		if (x == 0) break;
		int d = pre[x][besty];
		besty += DY[d];
	}
	return minCost;
}


void preProcess(FWH** &fwh, IplImage oriImg, CvSize size , CvSize minSize, CvSize maxSize){
	int oriW = size.with;
	int oriH = size.height;
	int minW = minSize.with;
	int minH = minSize.height;
	int maxW = maxSize.with;
	int maxH = maxSize.height;

	IpImage copyImage = image;

	fwh[oriH][oriW].cost = 0;

	
	for (int i = oriH ; i >= minH ; i-- ){
		for (int j = oriW ; j >= minW ; j--){
			FWH &fwht = fwh[i][j];
			if(i == oriH){
				if (j == minW ) continue;
				int cost = costDW(fwh[i][j+1].img, j+1 , i ,fwht.cutPath, fwht.img );
				fwht.cost += cost;
				fwht.setPre(i,j+1);
				continue;
			}
			if(i == oriW){
				
				continue;
			}

		}
	}

}


#endif