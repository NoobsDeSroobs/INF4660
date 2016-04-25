#ifndef STREAMLINE
#define STREAMLINE

#include <vector>
using std::vector;

#include "Integration.h"
#include "ReadData.h"

enum INTEGRATION_METHOD {EULER = 0, RK};

class Streamline{
	public:
		Streamline(int x, int y, int length, bool biDirectional, float stepSize,
				   INTEGRATION_METHOD integration, ReadData &reader);
		virtual ~Streamline();
		
		int getLength();
		float getStepSize();
		point getStartPoint();
		vector<point> getCurveForwardPoints();
		vector<point> getCurveBackwardPoints();
		vector<point> &getCurvePoints();
		
		bool isBiDirectional();
		bool isCriticalPoint();
		
	private:
		ReadData reader;
		int length;
		float stepSize;
		bool biDirectional;
		bool criticalPoint;
		INTEGRATION_METHOD integration;
		
		point startPoint;
		vector<point> curveForwardPoints;
		vector<point> curveBackwardPoints;
		vector<point> allCurvePoints;
		
		void calcCurve();
};

#endif
