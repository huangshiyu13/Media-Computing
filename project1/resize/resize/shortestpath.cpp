#include <ctime>
#include "shortestpath.h"

const int maxN = 1005;
const double mergeRatio = 0.6;

CvScalar cv2D[maxN][maxN];
int energy[maxN][maxN];
SPNode node[maxN * maxN];
SPEdge edge[maxN * maxN * 4];
int que[1 << 24];
int delpos[maxN];
CvScalar delcolor[maxN];
int edgeCnt;


ShortestPath::ShortestPath(IplImage *_image) {
	image = _image;
	H = image->height;
	W = image->width;
}

ShortestPath::~ShortestPath() {
	for (int i = 0; i < H; ++i) delete[] energy[i];
	delete[] energy;
}

int t[3][3] = {
	{1, 2, 1},
	{2, 0, 2},
	{1, 2, 1}
};

int ShortestPath::computeEnergy(int h, int w) {
	double en = 0;
	double to = 0;

	/*for (int i = -1; i <= 1; ++i)
		for (int j = -1; j <= 1; ++j) {
			int hh = h + i;
			int ww = w + j;
			if (hh < 0 || hh >= H) continue;
			if (ww < 0 || ww >= W) continue;
			en += ColorDist( cv2D[h][w], cv2D[hh][ww] ) * t[i + 1][j + 1];
			to += t[i + 1][j + 1];
		}
	en /= to;*/

	if (w == 0) {
		en += dist( cv2D[h][w], cv2D[h][w + 1] );
	} else if (w == W - 1) {
		en += dist( cv2D[h][w - 1], cv2D[h][w] );
	} else {
		en += dist( cv2D[h][w - 1], cv2D[h][w] );
		en += dist( cv2D[h][w], cv2D[h][w + 1] );
		en /= 2;
	}
	return en;
}

void ShortestPath::addEdge(int u, int v, int c) {
	int i = ++edgeCnt;
	edge[i].v = v;
	edge[i].c = c;
	edge[i].next = node[u].head;
	node[u].head = i;
}

void ShortestPath::mergeColor(int h, int w, CvScalar cc) {
	if (w == 0) {
		CvScalar cr = cv2D[h][w];
		cv2D[h][w] = cr * (1 - mergeRatio) + cc * mergeRatio;
	} else if (w == W - 1) {
		CvScalar cl = cv2D[h][w - 1];
		cv2D[h][w - 1] = cl * (1 - mergeRatio) + cc * mergeRatio;
	} else {
		CvScalar cl = cv2D[h][w - 1];
		CvScalar cr = cv2D[h][w];
		cv2D[h][w - 1] = cl * (1 - mergeRatio * 0.5) + cc * mergeRatio * 0.5;
		cv2D[h][w]     = cr * (1 - mergeRatio * 0.5) + cc * mergeRatio * 0.5;
	}
}

void ShortestPath::seamCarvingW() {

	double startTime = clock();

	//	调出颜色
	for (int h = 0; h < H; ++h)
		for (int w = 0; w < W; ++w)
			cv2D[h][w] = cvGet2D(image, h, w);

	//	计算能量
	for (int h = 0; h < H; ++h)
		for (int w = 0; w < W; ++w)
			energy[h][w] = computeEnergy(h, w);
	
	//	建图
	int s = H * W;
	int t = s + 1;
	for (int i = 0; i <= t; ++i) {
		node[i].head = 0;
		node[i].dist = 0x7FFFFFFF;
		node[i].pre = -1;
		node[i].inque = 0;
	}
	edgeCnt = 0;
	for (int j = 0; j < W; ++j) {
		addEdge( s, 0 * W + j, energy[0][j] );
		addEdge( (H - 1) * W + j, t, 0 );
	}
	for (int i = 1; i < H; ++i)
		for (int j = 0; j < W; ++j) {
			if (j > 0)     addEdge( (i - 1) * W + j, i * W + j - 1, energy[i][j - 1] );
			               addEdge( (i - 1) * W + j, i * W + j,     energy[i][j]     );
			if (j < W - 1) addEdge( (i - 1) * W + j, i * W + j + 1, energy[i][j + 1] );
		}
	//printf("Edge: %d\n", edgeCnt);
		
	//	最短路
	int la = 0;
	que[++la] = s;
	node[s].dist = 0;
	node[s].inque = 1;
	for (int fi = 1; fi <= la; ++fi) {
		int u = que[fi];
		for (int i = node[u].head; i; i = edge[i].next) {
			int v = edge[i].v;
			int c = edge[i].c;
			if (node[v].dist <= node[u].dist + c) continue;
			node[v].dist = node[u].dist + c;
			node[v].pre = u;
			if (node[v].inque) continue;
			node[v].inque = 1;
			que[++la] = v;
		}
	}
	//printf("SPFA: %d\n", la, W * H);
	//printf("%d\n", node[t].dist);

	//	回溯找缝
	for (int h = H - 1, u = node[t].pre; h >= 0; --h, u = node[u].pre) {
		if (u / W != h) printf("Error #0 : u:%d h:%d\n", u, h);
		delpos[h] = u % W;
	}

	//	构造新图
	for (int h = 0; h < H; ++h) {
		delcolor[h] = cv2D[h][delpos[h]];
		for (int w = delpos[h]; w < W - 1; ++w) {
			CvScalar color = cv2D[h][w + 1];
			cv2D[h][w] = color;
		}
		cv2D[h][W - 1] = CV_RGB(0, 0, 0);
		//cvSet2D(image, h, del[h], CV_RGB(0, 0, 0));
	}
	for (int h = 0; h < H; ++h) {
		mergeColor(h, delpos[h], delcolor[h]);
	}

	//	画图
	for (int h = 0; h < H; ++h)
		for (int w = 0; w < W; ++w)
			cvSet2D(image, h, w, cv2D[h][w]);

	--W;

	printf("%.3lf\n", (clock() - startTime) / 1000.0);
}