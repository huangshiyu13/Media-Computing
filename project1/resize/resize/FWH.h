#ifndef FWH_H
#define FWH_H
#include "define.h"

class FWH{
	CutPath cutPath;
	int WPre;
	int HPre;
	int cost;
	IplImage img;
	void setPre(int W, int H){
		WPre = W;
		HPre = H;
	}
	
};


#endif