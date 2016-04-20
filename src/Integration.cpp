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