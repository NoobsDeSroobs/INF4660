#include "Integration.h"

#include "ReadData.h"

point Integrations::ForwardEuler(point u_prev, ReadData &reader, float stepSize){
	point u = point();
	
	vecData vData = findVectorDataForPoint(u_prev, reader);
	vecData vecNorm = vecData(vData.x / vData.length(), vData.y / vData.length());
	
	u.x = u_prev.x + stepSize * vecNorm.x;
	u.y = u_prev.y + stepSize * vecNorm.y;
	
	return u;
}

point Integrations::RungeKutta(point u_prev, ReadData &reader, float stepSize){
	point u = point();
	point tempStep = point();
	vecData vData, vecNorm;
	
	//Find, normalize and treat the vector for each k step
	vData = findVectorDataForPoint(u_prev, reader);
	vecNorm = vecData(vData.x / vData.length(), vData.y / vData.length());
	point k_1 = point(vecNorm.x * stepSize, vecNorm.y * stepSize);
	
	tempStep.x = u_prev.x + k_1.x * 0.5; 
	tempStep.y = u_prev.y + k_1.y * 0.5; 
	vData = findVectorDataForPoint(tempStep, reader);
	vecNorm = vecData(vData.x / vData.length(), vData.y / vData.length());
	point k_2 = point(vecNorm.x * stepSize, vecNorm.y * stepSize);
	
	tempStep.x = u_prev.x + k_2.x * 0.5; 
	tempStep.y = u_prev.y + k_2.y * 0.5; 
	vData = findVectorDataForPoint(tempStep, reader);
	vecNorm = vecData(vData.x / vData.length(), vData.y / vData.length());
	point k_3 = point(vecNorm.x * stepSize, vecNorm.y * stepSize);
	
	tempStep.x = u_prev.x + k_3.x; 
	tempStep.y = u_prev.y + k_3.y; 
	vData = findVectorDataForPoint(tempStep, reader);
	vecNorm = vecData(vData.x / vData.length(), vData.y / vData.length());
	point k_4 = point(vecNorm.x * stepSize, vecNorm.y * stepSize);
	
	u.x = u_prev.x + k_1.x/6 + k_2.x/3 + k_3.x/3 + k_4.x/6;
	u.x = u_prev.y + k_1.y/6 + k_2.y/3 + k_3.y/3 + k_4.y/6;
	
	return u;
}

vecData Integrations::findVectorDataForPoint(point dataPoint, ReadData &reader){
	//Assumes given point coords are in the vector field coordinates
	//As such, if both x and y are integers, then it is a known 
	//field value. 
	if((dataPoint.x - (int)dataPoint.x == 0.0f) && (dataPoint.y - (int)dataPoint.y) == 0.0f){
		return reader.getVector(dataPoint.x, dataPoint.y);
	}
	
	//Otherwise they need to be interpolated
	int x1 = (int)dataPoint.x;
	int x2 = (int)dataPoint.x + 1;
	int y1 = (int)dataPoint.y;
	int y2 = (int)dataPoint.y + 1;
	
	return interpolateVectorData(x1, x2, y1, y2, dataPoint.x, dataPoint.y, reader);
}
vecData Integrations::interpolateVectorData(int x1, int x2, int y1, int y2, int x, int y,
											ReadData &reader){
	vecData result;
	vecData Q11 = reader.getVector(x1, y1);
	vecData Q12 = reader.getVector(x1, y2);
	vecData Q21 = reader.getVector(x2, y1);
	vecData Q22 = reader.getVector(x2, y2);
	
	//X component
	float V_xy1 = (x2 - x) / (x2 - x1) * Q11.x + (x - x1) / (x2 - x1) * Q21.x;
	float V_xy2 = (x2 - x) / (x2 - x1) * Q12.x + (x - x1) / (x2 - x1) * Q22.x;
	
	result.x = (y2 - y) / (y2 - y1) * V_xy1 + (y - y1) / (y2 - y1) * V_xy2;
	 
	//Y component
	V_xy1 = (x2 - x) / (x2 - x1) * Q11.x + (x - x1) / (x2 - x1) * Q21.x;
	V_xy2 = (x2 - x) / (x2 - x1) * Q12.x + (x - x1) / (x2 - x1) * Q22.x;
	
	result.y = (y2 - y) / (y2 - y1) * V_xy1 + (y - y1) / (y2 - y1) * V_xy2;
	
	result.trueData = false;
	return result;
}