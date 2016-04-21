#include "Streamline.h"

#include "Integration.h"

Streamline::Streamline(int x, int y, int length, bool biDirectional, float stepSize,
					   int euler){
	startPoint = point(x, y);
	this->length = length;
	this->biDirectional = biDirectional;
	this->stepSize = stepSize;
	this->euler = euler;
	
	calcCurve();
}
Streamline::~Streamline(){
	
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
		activeVector = findVectorDataForPoint(incForward);
		if(euler == 0){
			incForward = Integrations::ForwardEuler(incForward, activeVector, stepSize);
		}
		else{
			incForward = Integrations::RungeKutta(incForward, activeVector, stepSize);
		}
		curveForwardPoints[i] = point(incForward.x, incForward.y);
		
		if(biDirectional){
			activeVector = findVectorDataForPoint(incBackward);
			if(euler == 0){
				incBackward = Integrations::ForwardEuler(incForward, activeVector, stepSize);
			}
			else{
				incBackward = Integrations::RungeKutta(incForward, activeVector, stepSize);
			}
			curveBackwardPoints[i] = point(incBackward.x, incBackward.y);
		}
	}
}

vecData Streamline::findVectorDataForPoint(point dataPoint){
	//Assumes given point coords are in the vector field coordinates
	//As such, if both x and y are integers, then they are  a known 
	//field value. 
	if((dataPoint.x - (int)dataPoint.x == 0.0f) && (dataPoint.y - (int)dataPoint.y) == 0.0f){
		return vecData(4, 4);//getVectorData(dataPoint.x, dataPoint.y);
	}
	
	//Otherwise they need to be interpolated
//	Q1y
//	Q2y
//
//	Qx1
//	Qx2
	
	
	
	
	
	
//	vecData interpolateFieldValues(int x1, int x2, int y1, int y2, int x, int y){
//		ReadData reader;
//
//		vecData result;
//		vecData Q11 = reader.getVector(x1, y1);
//		vecData Q12 = reader.getVector(x1, y2);
//		vecData Q21 = reader.getVector(x2, y1);
//		vecData Q22 = reader.getVector(x2, y2);
//
//		//X component
//		float V_xy1 = (x2 - x) / (x2 - x1) * Q11.x + (x - x1) / (x2 - x1) * Q21.x;
//		float V_xy2 = (x2 - x) / (x2 - x1) * Q12.x + (x - x1) / (x2 - x1) * Q22.x;
//
//		result.x = (y2 - y) / (y2 - y1) * V_xy1 + (y - y1) / (y2 - y1) * V_xy2;
//
//		//Y component
//		V_xy1 = (x2 - x) / (x2 - x1) * Q11.x + (x - x1) / (x2 - x1) * Q21.x;
//		V_xy2 = (x2 - x) / (x2 - x1) * Q12.x + (x - x1) / (x2 - x1) * Q22.x;
//
//		result.y = (y2 - y) / (y2 - y1) * V_xy1 + (y - y1) / (y2 - y1) * V_xy2;
//
//		result.trueData = false;
//		return result;
//	}
}

std::vector<point> Streamline::getCurvePoints(){
	return std::vector<point>();
}

vecData getVectorData(int x, int y){
	return vecData(10, 5);
}
