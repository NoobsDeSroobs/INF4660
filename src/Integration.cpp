#include "Integration.h"

//Fills x_n+1 and y_n+1
point Integrations::ForwardEuler(point u_n, vecData vComp, float stepSize){
	//float u = u_n + stepSize * vComp;
	
	//return u;
	return point();
}

point Integrations::RungeKutta(point u_n, vecData vComp, float stepSize){
	//Burde vi normalisere vektor
	/*
	float vCompHalfDelta = vCompDelta / 2;
	
	float k_1 = vComp * stepSize;
	float k_2 = vCompHalfDelta * k_1;
	float k_3 = vCompHalfDelta * k_2;
	float k_4 = vCompDelta * k_3;
	
	float u = u_n + stepSize/6*(k_1 + 2*k_2 + 2*k_3 + k_4);
	return u;
	*/
	return point();
}