#ifndef STREAMLINE
#define STREAMLINE

#include <vector>
using std::vector;

#include "Integration.h"
#include "ReadData.h"

class Streamline{
	public:
		Streamline(int x, int y, int length, bool biDirectional, float stepSize,
				   int euler, ReadData reader);
		virtual ~Streamline();
		
		int getLength();
		float getStepSize();
		vector<point> getCurvePoints();
		
		bool isBiDirectional();
		
	private:
		ReadData reader;
		
		int length;
		float stepSize;
		bool biDirectional;
		int euler = 0;
		
		point startPoint;
		vector<point> curveForwardPoints;
		vector<point> curveBackwardPoints;
		
		void calcCurve();
		
		vecData findVectorDataForPoint(point pixelPoint);
		vecData getVectorData(int x, int y);
		vecData interpolateVectorData(int x1, int x2, int y1, int y2, int x, int y);
};

#endif
