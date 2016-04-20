#include "Integration.h"

Integrations::Integrations(){}
Integrations::~Integrations(){}

//Fills x_n+1 and y_n+1
float Integrations::ForwardEuler(float u_n, float vComp, float stepSize){
	float u = u_n + stepSize * vComp;
	
	return u;
}

float Integrations::RungeKutta(float u_n, float stepSize,
							   float vComp, float vCompDelta){
	float vCompHalfDelta = vCompDelta / 2;
	
	float k_1 = vComp;
	float k_2 = vCompHalfDelta * k_1;
	float k_3 = vCompHalfDelta * k_2;
	float k_4 = vCompDelta * k_3;
	
	float u = u_n + stepSize/6*(k_1 + 2*k_2 + 2*k_3 + k_4);
	return u;
}
/*
vecData interpolateFieldValues(int x1, int x2, int y1, int y2, int x, int y){
	//Temp
	ReadData reader;
	
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
*/