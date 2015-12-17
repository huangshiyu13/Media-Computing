#ifndef FWH_H
#define FWH_H
#include "define.h"
#include <opencv2/opencv.hpp>
class FWH{
public:
	CutPath cutPath;
	int WPre;
	int HPre;
	int cost;
	IplImage* img;
	void setPre(int W, int H){
		WPre = W;
		HPre = H;
	}
	changeType type;
};


#endif