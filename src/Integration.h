#ifndef INTEGRATIONS
#define INTEGRATIONS

#include <cmath>
class ReadData;

struct point{
	float x;
	float y;
	
	point(){
		x = 0.0;
		y = 0.0;
	}
	
	point(int x, int y): x(x), y(y){
	}

	float distanceTo(point other){
		point temp(other.x - x, other.y - y);
		return std::abs(temp.length());
	}

	float length(){
		return sqrt(pow(x, 2) + pow(y, 2));
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
} typedef velVector;

class Integrations{
	public:
		static point ForwardEuler(point u_prev, ReadData &reader, float stepSize);
		static point RungeKutta(point u_prev, ReadData &reader, float stepSize);
		
	private:
		static bool findVectorDataForPoint(point dataPoint, vecData &returnVec, ReadData &reader);
		static vecData interpolateVectorData(int x1, int x2, int y1, int y2, int x, int y,
											 ReadData &reader);
};

#endif
