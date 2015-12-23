#ifndef DEFINE_H
#define DEFINE_H
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
enum cutImageState{cutclean,drawing,circled,grabcuted};
enum bakImageState{bakclean,bottondown,bottonup};


//	if editing region is selected
bool ringed = false;

//	if poisson image editing is finished
bool poissonImageEditinged = false;

//	start coordinate of editing region selecting
int ringStartX, ringStartY;

//	last coordinate while editing region selecting
int ringLastX = -1, ringLastY = -1;

//	boundary path of editing region
vector< pair<int,int> > ringPath;

//	if a pixel is visited
bool **visit;

//	BFS queue
pair<int,int> *visitQue;

//	if now connected block include outside 
bool visitOut;

//	pixels of now connected block
vector<pair<int,int>> visitRegion;

//	matrix for id of a pixel while poisson image editing
int **id;

//	color matrix
CvScalar **c;



//	maximum image size
#define MAX_IMAGE_WIDTH 1200
#define MAX_IMAGE_HEIGHT 800

//	blending gradient or blending pixel
//#define BLENDING_GRADIENT

//	get coordinate in bakImg using coordinate in cutImg
#define TARX(x) (posx-msk->cx+(x))
#define TARY(y) (posy-msk->cy+(y))

//	blending coefficient
#define SOURCE_RATIO 0.9

//	get inverse of a color
#define INVCOLOR(x) (CV_RGB(255, 255,255)-x)


int dn = 4;
int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};


CvScalar operator + (CvScalar A, CvScalar B) {
	for (int k = 0; k < 3; ++k) A.val[k] += B.val[k];
	return A;
}

CvScalar operator - (CvScalar A, CvScalar B) {
	for (int k = 0; k < 3; ++k) A.val[k] -= B.val[k];
	return A;
}

CvScalar operator * (CvScalar A, double B) {
	for (int k = 0; k < 3; ++k) A.val[k] *= B;
	return A;
}


#endif