#ifndef PREPROCESS_H
#define PREPROCESS_H
#include <opencv2/opencv.hpp>
#include <windows.h>

#include "FWH.h"
#define LXN 3
#define LYN 3
const int LX[] = {-1, 0, 1};
const int LY[] = {-1, 0, 1};

CvScalar myCV2D[1000][1000];
double f[1000][1000];
int pre[1000][1000];

int costDW(IplImage* Img, int W , int H, CutPath &cutPath, IplImage* &reImg){
	
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
		cutPath.push_back(bestx);
		if (y == 0) break;
		int d = pre[bestx][y];
		bestx += LX[d];
	}
	

	return minCost;
}

int costDH(IplImage* &Img, int W , int H, CutPath &cutPath){
	

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
			cvSet2D(Img, y, x, myCV2D[x][y + 1]);
		}
		cutPath.push_back(besty);
		if (x == 0) break;
		int d = pre[x][besty];
		besty += LY[d];
	}
	//cout << "in6"<<endl;
	
	
	//cout << "in7"<<endl;
	return minCost;
}

int costIW(IplImage* Img, int W , int H, CutPath &cutPath, IplImage* &reImg){
	
	
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
		cutPath.push_back(bestx);
		if (y == 0) break;
		int d = pre[bestx][y];
		bestx += LX[d];
	}
	
	

	return minCost;
}

int costIH(IplImage* &Img, int W , int H, CutPath &cutPath){
	
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
			cvSet2D(Img, y, x, myCV2D[x][y + 1]);
		}
		cutPath.push_back(besty);
		if (x == 0) break;
		int d = pre[x][besty];
		besty += LY[d];
	}
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
	
	IplImage** imgs = new IplImage*[maxSize.width+1];

	imgs[oriW] = cvCloneImage(oriImg);
	CutPath path1,path2,zeroPath;
	for (int i = oriH ; i >= minH ; i-- ){
		if(i%10 == 0) cout << i << endl;
		for (int j = oriW ; j >= minW ; j--){
			//cout << "in1"<<endl;
			FWH &fwht = fwh[i][j];
			if(i == oriH){
				//cout << "in2"<<endl;
				if (j == oriW ) continue;
				int cost = costDW(imgs[j+1], j+1 , i ,fwht.cutPath, imgs[j] );
				fwht.cost = fwh[i][j+1].cost + cost;
				fwht.setPre(j+1, i);
				//cout << "in4"<<endl;
				fwht.type = ReduceW;
				continue;
			}
			if(j == oriW){
				//cout << "in5"<<endl;
				int cost = costDH(imgs[j], j , i+1 ,fwht.cutPath);
				fwht.cost = fwh[i+1][j].cost+cost;
				fwht.setPre(j,i+1);
				fwht.type = ReduceH;
				continue;
			}
			//cout << "in7"<<endl;
			
			IplImage* temp2 = cvCloneImage(imgs[j]);
			IplImage* temp3;
			
			
			int cost1 = costDW(imgs[j+1], j+1 , i ,path1,temp3 );
			int cost11 = fwh[i][j+1].cost + cost1;
		
			int cost2 = costDH(temp2, j , i+1 ,path2 );
			int cost22 = fwh[i+1][j].cost + cost2;
			
			if (cost11 < cost22){
				
				fwht.cost = cost11;
				fwht.setPre(j+1,i);
				fwht.type = ReduceW;
				fwht.cutPath.assign(path1.begin(), path1.end());
				cvReleaseImage(&imgs[j]);
				imgs[j] = cvCloneImage(temp3);
			}
			else{
				fwht.cost = cost22;
				fwht.setPre(j,i+1);
				fwht.type = ReduceH;
				fwht.cutPath.assign(path2.begin(), path2.end());
				cvReleaseImage(&imgs[j]);
				imgs[j] = cvCloneImage(temp2);
			}
			path1.swap(vector<int> ());
			path2.swap(vector<int> ());
			cvReleaseImage(&temp2);
			cvReleaseImage(&temp3);
		}
	}
	delete []imgs;
}

#endif