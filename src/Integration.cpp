#include "Integration.h"

#include "ReadData.h"

WMpoint Integrations::ForwardEuler(WMpoint u_prev, ReadData &reader, float stepSize){
	WMpoint u = WMpoint();
	
	vecData vData;
	if(!findVectorDataForPoint(u_prev, vData, reader)){
		u.isValid = false;
		return u;
	}

	//fprintf(stderr, "Forward: Point: x: %f, y: %f > Vector: x: %f, y: %f\n", u_prev.x, u_prev.y, vData.x, vData.y);
	vecData vecNorm = vecData(vData.x / vData.length(), vData.y / vData.length());
	
	u.x = u_prev.x + stepSize * vecNorm.x;
	u.y = u_prev.y + stepSize * vecNorm.y;
	
	return u;
}
/*
WMpoint Integrations::RungeKutta(WMpoint u_prev, ReadData &reader, float stepSize){
	WMpoint u = WMpoint();
	WMpoint tempStep = WMpoint();
	vecData vData, vecNorm;
	
	//Find, normalize and treat the vector for each k step
	if(!findVectorDataForPoint(u_prev, vData, reader)){
		return u_prev;
	}
	vecNorm = vecData(vData.x / vData.length(), vData.y / vData.length());
	WMpoint k_1 = WMpoint(vecNorm.x * stepSize, vecNorm.y * stepSize);
	
	tempStep.x = u_prev.x + k_1.x * 0.5; 
	tempStep.y = u_prev.y + k_1.y * 0.5; 
	if(!findVectorDataForPoint(tempStep, vData, reader)){
		return u_prev;
	}
	vecNorm = vecData(vData.x / vData.length(), vData.y / vData.length());
	WMpoint k_2 = WMpoint(vecNorm.x * stepSize, vecNorm.y * stepSize);
	
	tempStep.x = u_prev.x + k_2.x * 0.5; 
	tempStep.y = u_prev.y + k_2.y * 0.5; 
	if(!findVectorDataForPoint(tempStep, vData, reader)){
		return u_prev;
	}
	vecNorm = vecData(vData.x / vData.length(), vData.y / vData.length());
	WMpoint k_3 = WMpoint(vecNorm.x * stepSize, vecNorm.y * stepSize);
	
	tempStep.x = u_prev.x + k_3.x; 
	tempStep.y = u_prev.y + k_3.y; 
	if(!findVectorDataForPoint(tempStep, vData, reader)){
		return u_prev;
	}
	vecNorm = vecData(vData.x / vData.length(), vData.y / vData.length());
	WMpoint k_4 = WMpoint(vecNorm.x * stepSize, vecNorm.y * stepSize);
	
	u.x = u_prev.x + k_1.x/6 + k_2.x/3 + k_3.x/3 + k_4.x/6;
	u.x = u_prev.y + k_1.y/6 + k_2.y/3 + k_3.y/3 + k_4.y/6;
	
	return u;
}*/


WMpoint Integrations::RungeKutta(WMpoint u_prev, ReadData &reader, float stepSize){
	vecData vDatak1, vDatak2, vDatak3, vDatak4;
	float T4x, k1x, k2x, k3x, k4x, T4y, k1y, k2y, k3y, k4y;
	WMpoint u = WMpoint();
	
	if(!findVectorDataForPoint(u_prev, vDatak1, reader)){
		u.isValid = false;
		return u;
	}

	float length = vDatak1.length();
	k1x = stepSize*(vDatak1.x/length);
	k1y = stepSize*(vDatak1.y/length);
	WMpoint u_k1(u_prev.x + (stepSize/2), u_prev.y + vDatak1.y*(stepSize/2));

	if(!findVectorDataForPoint(u_k1, vDatak2, reader)){
		u.isValid = false;
		return u;
	}

	length = vDatak2.length();
	k2x = stepSize*(vDatak2.x/length);
	k2y = stepSize*(vDatak2.y/length);
	WMpoint u_k2(u_prev.x + (stepSize/2), u_prev.y + vDatak2.y*(stepSize/2));

	if(!findVectorDataForPoint(u_k2, vDatak3, reader)){
		u.isValid = false;
		return u;
	}

	length = vDatak3.length();
	k3x = stepSize*(vDatak3.x/length);
	k3y = stepSize*(vDatak3.y/length);
	WMpoint u_k3(u_prev.x + stepSize, u_prev.y + vDatak3.y*stepSize);

	if(!findVectorDataForPoint(u_k3, vDatak4, reader)){
		u.isValid = false;
		return u;
	}

	length = vDatak4.length();
	k4x = stepSize*(vDatak3.x/length);
	k4y = stepSize*(vDatak3.y/length);

	T4x = (k1x + 2*k2x + 2*k3x + k4x)/6;
	T4y = (k1y + 2*k2y + 2*k3y + k4y)/6;

	u = WMpoint(u_prev.x + T4x, u_prev.y + T4y);
	return u;
}

bool Integrations::findVectorDataForPoint(WMpoint dataPoint, vecData &returnVec, ReadData &reader){
	//Otherwise they need to be interpolated
	int x1 = (int)dataPoint.x;
	int x2 = (int)dataPoint.x + 1;
	int y1 = (int)dataPoint.y;
	int y2 = (int)dataPoint.y + 1;
	
	//If either x2 or y2 are outside the vector field, then the calculations must
	//stop here.
	if(x2 >= reader.getWidth() || y2 >= reader.getHeight()){
		return false;
	}
	
	return BilinearInterpolation(returnVec, x1, x2, y1, y2, dataPoint.x, dataPoint.y, reader);
}

vecData Integrations::interpolateVectorData(int x1, int x2, int y1, int y2, int x, int y,
											ReadData &reader){
	vecData result;
	vecData Q11 = reader.getVector(x1, y1);
	vecData Q12 = reader.getVector(x1, y2);
	vecData Q21 = reader.getVector(x2, y1);
	vecData Q22 = reader.getVector(x2, y2);
	
	//X component
	float V_xy1 = ((x2 - x) / (x2 - x1)) * Q11.x + ((x - x1) / (x2 - x1)) * Q21.x;
	float V_xy2 = ((x2 - x) / (x2 - x1)) * Q12.x + ((x - x1) / (x2 - x1)) * Q22.x;
	
	result.x = ((y2 - y) / (y2 - y1)) * V_xy1 + ((y - y1) / (y2 - y1)) * V_xy2;
	 
	//Y component
	V_xy1 = ((x2 - x) / (x2 - x1)) * Q11.x + ((x - x1) / (x2 - x1)) * Q21.x;
	V_xy2 = ((x2 - x) / (x2 - x1)) * Q12.x + ((x - x1) / (x2 - x1)) * Q22.x;
	
	result.y = ((y2 - y) / (y2 - y1)) * V_xy1 + ((y - y1) / (y2 - y1)) * V_xy2;
	
	result.trueData = false;
	return result;
}

float calcComponent(float q11, float q12, float q21, float q22, float x1, float x2, float y1, float y2, float x, float y){
	float comp;
	float x2x1, y2y1, x2x, y2y, yy1, xx1;
	
	x2x1 = x2 - x1;
	y2y1 = y2 - y1;
	x2x = x2 - x;
	y2y = y2 - y;
	yy1 = y - y1;
	xx1 = x - x1;
	comp = 1.0 / (x2x1 * y2y1) * (
		q11 * x2x * y2y +
		q21 * xx1 * y2y +
		q12 * x2x * yy1 +
		122 * xx1 * yy1
	);
	return comp;
}

bool Integrations::BilinearInterpolation(vecData &returnVec, float x1, float x2, float 
										 y1, float y2, float x, float y, ReadData &reader){
	vecData Q11 = reader.getVector(x1, y1);
	vecData Q12 = reader.getVector(x1, y2);
	vecData Q21 = reader.getVector(x2, y1);
	vecData Q22 = reader.getVector(x2, y2);	
	
	//Checks if any of the four points are critical points.
	if( (Q11.x == 0.0 && Q11.y == 0.0) || (Q12.x == 0.0 && Q12.y == 0.0) ||
		(Q21.x == 0.0 && Q21.y == 0.0) || (Q22.x == 0.0 && Q22.y == 0.0) ){
		return false;
	}
	
	//Calculates the new vector components
	returnVec.x = calcComponent(Q11.x, Q12.x, Q21.x, Q22.x, x1, x2, y1, y2, x, y);
	returnVec.y = calcComponent(Q11.y, Q12.y, Q21.y, Q22.y, x1, x2, y1, y2, x, y);
	return true;
}
