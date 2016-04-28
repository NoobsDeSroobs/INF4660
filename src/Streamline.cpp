#include "Streamline.h"

#include "Integration.h"
#include "ReadData.h"

Streamline::Streamline(float x, float y, int length, bool biDirectional, float stepSize,
					   INTEGRATION_METHOD integration, ReadData &reader){
	startPoint = WMpoint(x, y);
	this->length = length;
	this->biDirectional = biDirectional;
	this->stepSize = stepSize;
	this->integration = integration;
	this->reader = reader;
	
	calcCurve();
}
Streamline::~Streamline(){
	
}

WMpoint Streamline::getStartPoint(){
	return startPoint;
}
vector<WMpoint> Streamline::getCurveForwardPoints(){
	return curveForwardPoints;
}
vector<WMpoint> Streamline::getCurveBackwardPoints(){
	return curveBackwardPoints;
}
vector<WMpoint> Streamline::getCurvePoints(){
	return allCurvePoints;
}

int Streamline::getLength(){
	return length;
}
bool Streamline::isBiDirectional(){
	return biDirectional;
}
bool Streamline::isCriticalPoint(){
	return criticalPoint;
}

void Streamline::calcCurve(){
	WMpoint increment;	
	curveForwardPoints.resize(length, WMpoint());
	if(biDirectional){
		curveBackwardPoints.resize(length, WMpoint());
	}
	
	increment = startPoint;
	for(int i = 0; i < length; i++){
		if(integration == EULER){
			increment = Integrations::ForwardEuler(increment, reader, stepSize);
		}
		else{
			increment = Integrations::RungeKutta(increment, reader, stepSize);
		}
		if(!increment.isValid){
			curveForwardPoints.resize(i);
			break;//End point, do not store more of, or continue this direction
		}
		curveForwardPoints[i] = WMpoint(increment.x, increment.y);
	}
	
	increment = startPoint;
	for(int i = 0; i < length; i++){
		if(biDirectional){
			if(integration == EULER){
				increment = Integrations::ForwardEuler(increment, reader, -stepSize);
			}
			else{
				increment = Integrations::RungeKutta(increment, reader, -stepSize);
			}
			if(!increment.isValid){
				curveBackwardPoints.resize(i);
				break;//End point, do not store more of, or continue this direction
			}
			curveBackwardPoints[i] = WMpoint(increment.x, increment.y);
		}
	}

	if(biDirectional){
		for (uint i = curveBackwardPoints.size(); i > 0; i--) {
			allCurvePoints.push_back(curveBackwardPoints[i-1]);
		}
	}
	allCurvePoints.push_back(startPoint);
	for (uint i = 0; i < curveForwardPoints.size(); ++i) {
		allCurvePoints.push_back(curveForwardPoints[i]);
	}
}
