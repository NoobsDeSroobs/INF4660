#include "Streamline.h"

#include "Integration.h"
#include "ReadData.h"

Streamline::Streamline(int x, int y, int length, bool biDirectional, float stepSize,
					   INTEGRATION_METHOD integration, ReadData &reader){
	startPoint = WMpoint(x, y);
	this->length = length;
	this->biDirectional = biDirectional;
	this->stepSize = stepSize;
	this->integration = integration;
	this->reader = reader;
	
	//Critical point handling and curve calculation
	vecData vec = reader.getVector(x, y);
	if(vec.x == 0 && vec.y == 0){
		criticalPoint = true;
	}
	else{
		criticalPoint = false;
		calcCurve();
	}
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
	WMpoint increment, tempIncr;
	curveForwardPoints.resize(length, WMpoint());
	if(biDirectional){
		curveBackwardPoints.resize(length, WMpoint());
	}

	increment = startPoint;
	for(int i = 0; i < length; i++){
		tempIncr = increment;
		if(integration == EULER){
			increment = Integrations::ForwardEuler(increment, reader, stepSize);
		}
		else{
			increment = Integrations::RungeKutta(increment, reader, stepSize);
		}
		
		if((increment.x == tempIncr.x && increment.y == tempIncr.y)	||
		   (increment.x >= reader.getWidth() || increment.y >= reader.getHeight())	||
		   (increment.x < 0 || increment.y < 0)){
			curveForwardPoints.resize(i);
			break;//End point, do not store more of, or continue this direction
		}

		curveForwardPoints[i] = WMpoint(increment.x, increment.y);
	}
	
	increment = startPoint;
	for(int i = 0; i < length; i++){
		tempIncr = increment;
		if(biDirectional){
			if(integration == EULER){
				increment = Integrations::ForwardEuler(increment, reader, -stepSize);
			}
			else{
				increment = Integrations::RungeKutta(increment, reader, -stepSize);
			}
			
			if((increment.x == tempIncr.x && increment.y == tempIncr.y)	||
			   (increment.x >= reader.getWidth() || increment.y >= reader.getHeight())	||
			   (increment.x < 0 || increment.y < 0)){
				curveForwardPoints.resize(i);
				break;//End point, do not store more of, or continue this direction
			}
			curveBackwardPoints[i] = WMpoint(increment.x, increment.y);
		}
	}
	
	if(biDirectional){
		for (int i = curveBackwardPoints.size()-1; i >= 0; --i) {
			allCurvePoints.push_back(curveBackwardPoints[i]);
		}
	}
	allCurvePoints.push_back(startPoint);
	for (int i = 0; i < curveForwardPoints.size(); ++i) {
		allCurvePoints.push_back(WMpoint(curveForwardPoints[i].x, curveForwardPoints[i].y));
	}
}
