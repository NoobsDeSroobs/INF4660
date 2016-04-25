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

point Streamline::getStartPoint(){
	return startPoint;
}
vector<point> Streamline::getCurveForwardPoints(){
	return curveForwardPoints;
}
vector<point> Streamline::getCurveBackwardPoints(){
	return curveBackwardPoints;
}
vector<point> &Streamline::getCurvePoints(){
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
	point increment, tempIncr;
	curveForwardPoints.resize(length-1, point());
	curveBackwardPoints.resize(length-1, point());
	

	increment = startPoint;
	for(int i = 0; i < length; i++){
		tempIncr = increment;
		if(integration == EULER){
			increment = Integrations::ForwardEuler(increment, reader, stepSize);
		}
		else{
			increment = Integrations::RungeKutta(increment, reader, stepSize);
		}
		
		if(increment.x == tempIncr.x && increment.y == tempIncr.y){
			break;//End point, do not store more of, or continue this direction
		}
		curveForwardPoints[i] = point(increment.x, increment.y);
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
			
			
			if(increment.x == tempIncr.x && increment.y == tempIncr.y){
				break;//End point, do not store more of, or continue this direction
			}
			curveBackwardPoints[i] = point(increment.x, increment.y);
		}
	}

	fprintf(stderr, "Before backwards print:\n");
	for (int i = curveBackwardPoints.size()-1; i >= 0; --i) {
		fprintf(stderr, "Point %d: %f, %f\n", i, curveBackwardPoints[i].x, curveBackwardPoints[i].y);
		allCurvePoints.push_back(curveBackwardPoints[i]);
	}
	fprintf(stderr, "Before forward print:\n");
	for (int i = 0; i < curveForwardPoints.size(); ++i) {
		fprintf(stderr, "Point %d: %f, %f\n", i, curveForwardPoints[i].x, curveForwardPoints[i].y);
		allCurvePoints.push_back(curveForwardPoints[i]);
	}
}
