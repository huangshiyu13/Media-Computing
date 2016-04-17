#ifndef GAUSSSEIDELSOLVER_H
#define GAUSSSEIDELSOLVER_H
#include <algorithm>
#include <ctime>
#include "BakImage.h"
#include "CutImage.h"
using namespace std;
typedef pair<int,double> GSIUnit;
const int MAX_GSIUNIT_IN_EQUATION = 5;
class GaussSeidelSolver {
public:
	GSIUnit **a;
	double *b;
	double *ans;
	int size;
	int nowEquation;
	int nowGSIUnitNum;
	GaussSeidelSolver(int size0) {
		srand(time(0));
		size = size0;
		a = new GSIUnit*[size];
		for (int i = 0; i < size; ++i) {
			a[i] = new GSIUnit[MAX_GSIUNIT_IN_EQUATION];
			for (int j = 0; j < MAX_GSIUNIT_IN_EQUATION; ++j) a[i][j].second = 0;
		}
		b = new double[size];
		ans = new double[size];
	}
	~GaussSeidelSolver() {
		for (int i = 0; i < size; ++i) delete[] a[i];
		delete[] a;
	}
	void addEquation(int _nowEquation) {
		nowEquation = _nowEquation;
		nowGSIUnitNum = 0;
	}
	void addGSIUnit(int id, double value) {
		a[nowEquation][nowGSIUnitNum++] = make_pair(id, value);
	}
	void addB(double value) {
		b[nowEquation] = value;
	}
	double getInitialValue() {
		return rand() % 10000 / 10000.0 * 256;
	}
	void solve(int maxIterationTimes, int mode) {
		solve(maxIterationTimes, -1, mode);
	}

	void solve(double maxDifference, int mode) {
		solve(-1, maxDifference, mode);
	}
	void solve(int maxIterationTimes, double maxDifference, int mode) {
		for (int i = 0; i < size; ++i) ans[i] = getInitialValue();
		double *d = new double[size];
		for (int times = 0; times != maxIterationTimes; ++times) {
			bool ifSame = true;
			for (int i = 0; i < size; ++i) {
				d[i] = b[i];
				for (int j = 1; j < MAX_GSIUNIT_IN_EQUATION; ++j)
					d[i] -= a[i][j].second * ans[a[i][j].first];
				d[i] /= a[i][0].second;
				if (mode == 1) {
					ifSame &= (ans[i] - d[i]) < maxDifference;
					ans[i] = d[i];
				}
			}
			if (mode == 0)
				for (int i = 0; i < size; ++i) {
					ifSame &= (ans[i] - d[i]) < maxDifference;
					ans[i] = d[i];
				}
				if (ifSame) break;
		}
	}
};

#endif
