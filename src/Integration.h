#ifndef INTEGRATIONS
#define INTEGRATIONS

#include <math.h>

struct point{
	float x;
	float y;
	
	point(){
		x = 0.0;
		y = 0.0;
	}
	
	point(int x, int y): x(x), y(y){
	}
};

struct vecData{
	float x;
	float y;
	bool trueData;
	
	vecData(){
		x = 0;
		y = 0;
		trueData = false;
	}
	vecData(float x, float y): x(x), y(y), trueData(false){
	
	}
	
	float length(){
		return sqrt(pow(x, 2)+pow(y, 2));
	}
};

class Integrations{
	public:
		static point ForwardEuler(point u_n, vecData vComp, float stepSize);
		
		static point RungeKutta(point u_n, vecData vComp, float stepSize);
};

#endif