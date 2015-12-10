#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H


#include "basic.h"


//	edge in shortest path method
class SPEdge {
public:
	int v, c, next;
};

//	node in shortest path method
class SPNode {
public:
	int dist, pre, inque, head;
};

//	shortest path method
class ShortestPath {
public:
	IplImage *image;
	int H, W;

	ShortestPath(IplImage *);
	~ShortestPath();
	int computeEnergy(int, int);
	void addEdge(int, int, int);
	void mergeColor(int, int, CvScalar);
	void seamCarvingW();
	void seamCarvingH();
};


#endif