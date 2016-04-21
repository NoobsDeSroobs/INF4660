#ifndef STREAMLINE
#define STREAMLINE

#include <vector>
using std::vector;

#include "Integration.h"

class Streamline{
	public:
		Streamline(int x, int y, int length, bool biDirectional, float stepSize,
				   int euler = 0);
		virtual ~Streamline();
		
		int getLength();
		float getStepSize();
		vector<point> getCurvePoints();
		
		bool isBiDirectional();
		
	private:
		int length;
		float stepSize;
		bool biDirectional;
		int euler;
		
		point startPoint;
		vector<point> curveForwardPoints;
		vector<point> curveBackwardPoints;
		
		void calcCurve();
		
		vecData findVectorDataForPoint(point pixelPoint);
};

#endif
