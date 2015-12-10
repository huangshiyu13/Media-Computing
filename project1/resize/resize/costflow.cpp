#include <algorithm>
#include "costflow.h"
using namespace std;

#define ID(x,y,z) (((x)*height+(y))*2+(z))
#define EPS 1e-7
#define DXN 3
#define DYN 3

const int DX[] = {-1, 0, 1};
const int DY[] = {-1, 0, 1};


CFEdge::CFEdge(int _v, int _c, int _q, CFEdge *_next) {
	v = _v;
	c = _c;
	q = _q;
	next = _next;
}

CostFlow::CostFlow(IplImage *_image, CvSize size) {
	image = _image;
	width = size.width;
	height = size.height;

	s = width * height * 2;
	t = s + 1;
	len = t + 1;
	e = new CFEdge*[len];
	pre = new CFEdge*[len];
	thr = new CFEdge*[len];
	for (int i = 0; i < len; ++i) e[i] = pre[i] = thr[i] = NULL;
	d = new int[len];
	que.clear();
	inq = new bool[len];
}

CostFlow::~CostFlow() {
	for (int i = 0; i < len; ++i) deleteEdgeDFS(e[i]);
	delete[] e;
	delete[] pre;
	delete[] thr;
	delete[] d;
	delete[] inq;
}

void CostFlow::deleteEdgeDFS(CFEdge *edge) {
	if (edge == NULL) return;
	deleteEdgeDFS(edge->next);
	delete edge;
}

CFEdge* CostFlow::addEdge(int u, int v, int c, int q) {
	CFEdge *oriEdge = new CFEdge(v, c, q, e[u]);
	CFEdge *invEdge = new CFEdge(u, 0, -q, e[v]);
	e[u] = invEdge->oppo = oriEdge;
	e[v] = oriEdge->oppo = invEdge;
	return oriEdge;
}

void CostFlow::resizeWidth(int newWidth) {

	//	construct cost flow network
	for (int x = 0; x < width; ++x) {
		addEdge(s, ID(x, 0, 0), 1, 0);
		addEdge(ID(x, height - 1, 1), t, 1, 0);
	}

	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
			thr[ID(x, y, 0)] = addEdge(ID(x, y, 0), ID(x, y, 1), 1, 0);
	
	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height - 1; ++y)
			for (int d = 0; d < DXN; ++d) {
				int xx = x + DX[d];
				if (xx < 0 || xx >= width - 1) continue;
				addEdge(ID(x, y, 1), ID(xx, y + 1, 0), 1, (int)(dist(cvGet2D(image, y, x), cvGet2D(image, y + 1, xx))) + EPS);
			}
	
	//	solve cost flow problem
	int minCost = 0;
	for (int times = 0; times < abs(newWidth - width); ++times) {
		memset(d, 100, len * sizeof(int));
		memset(inq, 0, len * sizeof(bool));
		d[s] = 0;
		inq[s] = true;
		que.clear();
		que.push_back(s);
		for (int head = 0; head < que.size(); ++head) {
			int u = que[head];
			for (CFEdge *edge = e[u]; edge != NULL; edge = edge->next) {
				if (!edge->c) continue;
				int v = edge->v;
				if (d[v] <= d[u] + edge->q) continue;
				d[v] = d[u] + edge->q;
				pre[v] = edge;
				if (inq[v]) continue;
				inq[v] = true;
				que.push_back(v);
			}
			inq[u] = false;
		}
		int flow = 1<<30;
		for (int i = t; i != s; i = pre[i]->oppo->v) {
			flow = min(flow, pre[i]->c);
		}
		for (int i = t; i != s; i = pre[i]->oppo->v) {
			pre[i]->c -= flow;
			pre[i]->oppo->c += flow;
		}
		minCost += d[t] * flow;
	}
	
	//	shrink or expand
	if (newWidth < width) {
		for (int y = 0; y < height; ++y) {
			int xx = 0;
			for (int x = 0; x < width; ++x) {
				if (thr[ID(x, y, 0)]->c == 0) continue;
				cvSet2D(image, y, xx++, cvGet2D(image, y, x));
			}
			for (int x = newWidth; x < width; ++x) cvSet2D(image, y, x, CV_RGB(0, 0, 0));
		}
	} else {
		for (int y = 0; y < height; ++y) {
			int xx = newWidth - 1;
			for (int x = width - 1; x >= 0; --x) {
				cvSet2D(image, y, xx--, cvGet2D(image, y, x));
				if (thr[ID(x, y, 0)]->c == 1) continue;
				cvSet2D(image, y, xx--, cvGet2D(image, y, x));
			}
		}
	}
}

void CostFlow::resizeHeight(int newHeight) {
	for (int y = 0; y < height; ++y) {
		addEdge(s, ID(0, y, 0), 1, 0);
		addEdge(ID(width - 1, y, 1), t, 1, 0);
	}

	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
			thr[ID(x, y, 0)] = addEdge(ID(x, y, 0), ID(x, y, 1), 1, 0);
	
	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height - 1; ++y)
			for (int d = 0; d < DYN; ++d) {
				int yy = y + DX[d];
				if (yy < 0 || yy >= height - 1) continue;
				addEdge(ID(x, y, 1), ID(x + 1, yy, 0), 1, (int)(dist(cvGet2D(image, y, x), cvGet2D(image, yy, x + 1))) + EPS);
			}
	
	int minCost = 0;
	for (int times = 0; times < abs(newHeight - height); ++times) {
		memset(d, 100, len * sizeof(int));
		memset(inq, 0, len * sizeof(bool));
		d[s] = 0;
		inq[s] = true;
		que.clear();
		que.push_back(s);
		for (int head = 0; head < que.size(); ++head) {
			int u = que[head];
			for (CFEdge *edge = e[u]; edge != NULL; edge = edge->next) {
				if (!edge->c) continue;
				int v = edge->v;
				if (d[v] <= d[u] + edge->q) continue;
				d[v] = d[u] + edge->q;
				pre[v] = edge;
				if (inq[v]) continue;
				inq[v] = true;
				que.push_back(v);
			}
			inq[u] = false;
		}
		int flow = 1<<30;
		for (int i = t; i != s; i = pre[i]->oppo->v) {
			flow = min(flow, pre[i]->c);
		}
		for (int i = t; i != s; i = pre[i]->oppo->v) {
			pre[i]->c -= flow;
			pre[i]->oppo->c += flow;
		}
		minCost += d[t] * flow;
	}
	
	if (newHeight < height) {
		for (int x = 0; x < width; ++x) {
			int yy = 0;
			for (int y = 0; y < height; ++y) {
				if (thr[ID(x, y, 0)]->c == 0) continue;
				cvSet2D(image, yy++, x, cvGet2D(image, y, x));
			}
			for (int y = newHeight; y < height; ++y) cvSet2D(image, y, x, CV_RGB(0, 0, 0));
		}
	} else {
		for (int x = 0; x < width; ++x) {
			int yy = newHeight - 1;
			for (int y = height - 1; y >= 0; --y) {
				cvSet2D(image, yy--, x, cvGet2D(image, y, x));
				if (thr[ID(x, y, 0)]->c == 1) continue;
				cvSet2D(image, yy--, x, cvGet2D(image, y, x));
			}
		}
	}
}