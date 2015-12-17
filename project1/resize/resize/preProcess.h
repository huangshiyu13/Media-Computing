#ifndef PREPROCESS_H
#define PREPROCESS_H
#include <opencv2/opencv.hpp>
#include <windows.h>

#include "FWH.h"
#define LXN 3
#define LYN 3
const int LX[] = {-1, 0, 1};
const int LY[] = {-1, 0, 1};

CvScalar myCV2D[1200][900];

int costDW(IplImage* Img, int W , int H, CutPath &cutPath, IplImage* &reImg){
	double** f;
	int** pre;
	//cout << "inin1"<<endl;
	f = new double*[W];
	pre = new int*[W];
	for (int x = 0; x < W; ++x) {
		f[x] = new double[H];
		pre[x] = new int[H];
	}
	reImg = cvCloneImage(Img);
	//cout << "inin2"<<endl;
	for (int x = 0; x < W; ++x)
		for (int y = 0; y < H; ++y){
			//cout << x << " " << y << endl;
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
				if (xx < 0 || xx >= W) continue;
				double now = f[xx][y - 1] + dist(myCV2D[x][y], myCV2D[xx][y - 1]);
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
			minCost = f[bestx][H - 1];
	}


	for (int y = H - 1; y >= 0; --y) {
		for (int x = bestx; x < W - 1; ++x) {
			cvSet2D(reImg, y, x, myCV2D[x + 1][y]);
		}
		cutPath.push_back(cvSize(bestx,y));
		if (y == 0) break;
		int d = pre[bestx][y];
		bestx += LX[d];
	}
	for (int x = 0; x < W; ++x) {
			delete[] f[x];
			delete[] pre[x];
		}
	delete[] f;
	delete[] pre;

	return minCost;
}

int costDH(IplImage* Img, int W , int H, CutPath &cutPath , IplImage* &reImg){
	double** f;
	int** pre;
	//cout << "in1"<<endl;
	f = new double*[W];
	pre = new int*[W];
	for (int x = 0; x < W; ++x) {
		f[x] = new double[H];
		pre[x] = new int[H];
	}
	reImg = cvCloneImage(Img);
	//cout << "in2"<<endl;

	for (int x = 0; x < W; ++x)
		for (int y = 0; y < H; ++y){
			myCV2D[x][y] = cvGet2D(Img, y, x);
		}
			
//cout << "in3"<<endl;
	for (int y = 0; y < H; ++y) f[0][y] = 0;

	for (int x = 1; x < W; ++x)
		for (int y = 0; y < H; ++y) {
			f[x][y] = 1e20;
			for (int d = 0; d < LYN; ++d) {
				int yy = y + LY[d];
				if (yy < 0 || yy >= H) continue;
				double now = f[x - 1][yy] + dist(myCV2D[x][y], myCV2D[x - 1][yy]);
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
		minCost = f[W - 1][besty];
	}
	//cout << "in5"<<endl;
	for (int x = W - 1; x >= 0; --x) {
		for (int y = besty; y < H - 1; ++y) {
			cvSet2D(reImg, y, x, myCV2D[x][y + 1]);
		}
		cutPath.push_back(cvSize(x,besty));
		if (x == 0) break;
		int d = pre[x][besty];
		besty += LY[d];
	}
	//cout << "in6"<<endl;
	for (int x = 0; x < W; ++x) {
			delete[] f[x];
			delete[] pre[x];
		}
	delete[] f;
	delete[] pre;
	//cout << "in7"<<endl;
	return minCost;
}

void preProcess(FWH** &fwh, IplImage* oriImg, CvSize size , CvSize minSize, CvSize maxSize){
	int oriW = size.width;
	int oriH = size.height;
	int minW = minSize.width;
	int minH = minSize.height;
	int maxW = maxSize.width;
	int maxH = maxSize.height;
	
	fwh[oriH][oriW].cost = 0;
	fwh[oriH][oriW].img = oriImg;

	for (int i = oriH ; i >= minH ; i-- ){
		for (int j = oriW ; j >= minW ; j--){
			//cout << i << " " << j <<endl;
			//cout << "in1"<<endl;
			FWH &fwht = fwh[i][j];
			if(i == oriH){
				//cout << "in2"<<endl;
				if (j == oriW ) continue;
				int cost = costDW(fwh[i][j+1].img, j+1 , i ,fwht.cutPath, fwht.img );
				//cout << "in3"<<endl;
				//fwht.img = cvCloneImage(oriImg);
				fwht.cost = fwh[i][j+1].cost + cost;
				fwht.setPre(i,j+1);
				//cout << "in4"<<endl;
				fwht.type = ReduceW;
				continue;
			}
			if(j == oriW){
				//cout << "in5"<<endl;
				int cost = costDH(fwh[i+1][j].img, j , i+1 ,fwht.cutPath, fwht.img );
				fwht.cost = fwh[i+1][j].cost+cost;
				fwht.setPre(i+1,j);
				//fwht.img = cvCloneImage(oriImg);
				//cout << "in6"<<endl;
				fwht.type = ReduceH;
				continue;
			}
			//cout << "in7"<<endl;
			FWH fwhtCopy1;
			FWH fwhtCopy2;

			int cost1 = costDW(fwh[i][j+1].img, j+1 , i ,fwhtCopy1.cutPath, fwhtCopy1.img );
			fwhtCopy1.cost = fwh[i][j+1].cost + cost1;
			fwhtCopy1.setPre(i,j+1);
			fwhtCopy1.type = ReduceW;

			int cost2 = costDH(fwh[i+1][j].img, j , i+1 ,fwhtCopy2.cutPath, fwhtCopy2.img );
			fwhtCopy2.cost = fwh[i+1][j].cost + cost2;
			fwhtCopy2.setPre(i+1,j);
			fwhtCopy2.type = ReduceH;
			if (fwhtCopy1.cost < fwhtCopy2.cost){
				fwht.cost = fwhtCopy1.cost;
				costDW(fwh[i][j+1].img, j+1 , i ,fwht.cutPath, fwht.img );
				fwht.setPre(i,j+1);
				fwht.type = ReduceW;
			}
			else{
				fwht.cost = fwhtCopy2.cost;
				costDW(fwh[i+1][j].img, j , i+1 ,fwht.cutPath, fwht.img );
				fwht.setPre(i+1,j);
				fwht.type = ReduceH;
			}
			//fwht.img = cvCloneImage(oriImg);
			//cout << "in8"<<endl;
		}
	}
}


#endif