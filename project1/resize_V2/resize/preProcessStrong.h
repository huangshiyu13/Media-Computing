#ifndef PREPROCESSSTRONG_H
#define PREPROCESSSTRONG_H
#include <opencv2/opencv.hpp>
#include <windows.h>
#include "FWH.h"

void costDWS(IplImage* Img, int W , int H, CutPath &cutPath){
	//cout << "inin2"<<endl;
	for (int x = 0; x < W; ++x)
		for (int y = 0; y < H; ++y){
			myCV2D[x][y] = cvGet2D(Img, y, x);
		}
		//cout << "inin3"<<endl;
		for (int x = 0; x < W; ++x) f[x][0] = 0;
		//cout << "inin4"<<endl;
		for (int y = 1; y < H; ++y)
			for (int x = 0; x < W; ++x) {
				f[x][y] = 1e20;
				for (int d = 0; d < LXN; ++d) {
					int xx = x + LX[d];
					if (xx <= 0 || xx >= W-1) continue;
					double now = f[xx][y - 1] + dist(myCV2D[xx-1][y], myCV2D[xx+1][y])+dist(myCV2D[xx][y], myCV2D[xx+1][y ])+dist(myCV2D[xx-1][y], myCV2D[xx][y ]);
					if (now < f[x][y]) {
						f[x][y] = now;
						pre[x][y] = d;
					}
				}
			}

			int bestx = 0;
			int minCost = 0 ;
			for (int x = 0; x < W; ++x) {
				bestx = f[bestx][H - 1] < f[x][H - 1] ? bestx : x;
			}


			for (int y = H - 1; y >= 0; --y) {
				for (int x = bestx; x < W - 1; ++x) {
					cvSet2D(Img, y, x, myCV2D[x + 1][y]);
				}
				cutPath.push_back(bestx);
				if (y == 0) break;
				int d = pre[bestx][y];
				bestx += LX[d];
			}
}

void costDHS(IplImage* &Img, int W , int H, CutPath &cutPath){
	for (int x = 0; x < W; ++x)
		for (int y = 0; y < H; ++y){
			myCV2D[x][y] = cvGet2D(Img, y, x);
		}

	for (int y = 0; y < H; ++y) f[0][y] = 0;

	for (int x = 1; x < W; ++x)
		for (int y = 0; y < H; ++y) {
			f[x][y] = 1e20;
			for (int d = 0; d < LYN; ++d) {
				int yy = y + LY[d];
				if (yy <= 0 || yy >= H-1) continue;
				double now = f[x - 1][yy] + dist(myCV2D[x][yy-1], myCV2D[x][yy+1])+dist(myCV2D[x][yy-1], myCV2D[x][yy])+dist(myCV2D[x][yy], myCV2D[x][yy+1]);
				if (now < f[x][y]) {
					f[x][y] = now;
					pre[x][y] = d;
				}
			}
		}
		//cout << "in4"<<endl;
	int besty = 0;
	int minCost = 0;
	for (int y = 0; y < H; ++y) {
		besty = f[W - 1][besty] < f[W - 1][y] ? besty : y;
	}
	//cout << "in5"<<endl;
	for (int x = W - 1; x >= 0; --x) {
		for (int y = besty; y < H - 1; ++y) {
			cvSet2D(Img, y, x, myCV2D[x][y + 1]);
		}
		cutPath.push_back(besty);
		if (x == 0) break;
		int d = pre[x][besty];
		besty += LY[d];
	}
}

void preProcessWS(FWH* &fwh, IplImage* oriImg, CvSize size){
	int oriW = size.width;
	int oriH = size.height;

	IplImage* img = cvCloneImage(oriImg);

	for (int j = oriW-1 ; j >= 1 ; j--){
		FWH &fwht = fwh[j];
		costDWS(img, j+1 , oriH ,fwht.cutPath );
	}
	cvReleaseImage(&img);
}

void preProcessHS(FWH* &fwh, IplImage* oriImg, CvSize size){
	int oriW = size.width;
	int oriH = size.height;

	IplImage* img = cvCloneImage(oriImg);

	for (int j = oriH-1 ; j >= 1 ; j--){
		FWH &fwht = fwh[j];
		costDHS(img, oriW, j+1 ,fwht.cutPath );
	}
	cvReleaseImage(&img);
}


#endif