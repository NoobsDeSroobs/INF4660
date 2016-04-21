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
		vector<point> getCurvePoints();
		
		bool isBiDirectional();
		
	private:
		ReadData reader;
		int length;
		float stepSize;
		bool biDirectional;
		INTEGRATION_METHOD integration;
		
		point startPoint;
		vector<point> curveForwardPoints;
		vector<point> curveBackwardPoints;
		
		void calcCurve();
};

#endif