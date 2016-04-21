#include "Streamline.h"

#include "Integration.h"
#include "ReadData.h"

Streamline::Streamline(int x, int y, int length, bool biDirectional, float stepSize,
					   INTEGRATION_METHOD integration, ReadData &reader){
	startPoint = point(x, y);
	this->length = length;
	this->biDirectional = biDirectional;
	this->stepSize = stepSize;
	this->integration = integration;
	this->reader = reader;
	
	calcCurve();
}
Streamline::~Streamline(){
	
}

std::vector<point> Streamline::getCurvePoints(){
	return std::vector<point>();
}
int Streamline::getLength(){
	return length;
}
bool Streamline::isBiDirectional(){
	return biDirectional;
}
void Streamline::calcCurve(){
	point incForward, incBackward;
	vecData activeVector;
	curveForwardPoints.resize(length, point());
	curveBackwardPoints.resize(length, point());
	
	incBackward = incForward = startPoint;
	for(int i = 0; i < length; i++){
		if(integration == EULER){
			incForward = Integrations::ForwardEuler(incForward, reader, stepSize);
		}
		else{
			incForward = Integrations::RungeKutta(incForward, reader, stepSize);
		}
		curveForwardPoints[i] = point(incForward.x, incForward.y);
		
		if(biDirectional){
			if(integration == EULER){
				incBackward = Integrations::ForwardEuler(incBackward, reader, -stepSize);
			}
			else{
				incBackward = Integrations::RungeKutta(incBackward, reader, -stepSize);
			}
			curveBackwardPoints[i] = point(incBackward.x, incBackward.y);
		}
	}
}
