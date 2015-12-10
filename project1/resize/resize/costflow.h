#ifndef COSTFLOW_H
#define COSTFLOW_H


#include <vector>
#include "basic.h"
using namespace std;


//	edge in cost flow method
class CFEdge {
public:
	int v, c, q;
	CFEdge *next;
	CFEdge *oppo;

	CFEdge(int, int, int, CFEdge *);
};


//	cost flow method
class CostFlow {
public:
	IplImage *image;
	int width, height;

	CFEdge **e, **pre, **thr;
	int s, t, len, *d;
	vector<int> que;
	bool *inq;

	CostFlow(IplImage *, CvSize);
	~CostFlow();
	void deleteEdgeDFS(CFEdge *);
	CFEdge* addEdge(int, int, int, int);
	void resizeWidth(int);
	void resizeHeight(int);
};

#endif