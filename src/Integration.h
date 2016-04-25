#ifndef INTEGRATIONS
#define INTEGRATIONS

#include <cmath>
class ReadData;

struct WMpoint{
	float x;
	float y;
	
	WMpoint(){
		x = 0.0;
		y = 0.0;
	}
	
	WMpoint(float x, float y): x(x), y(y){
	}

	float distanceTo(WMpoint other){
		WMpoint temp(other.x - x, other.y - y);
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
		static WMpoint ForwardEuler(WMpoint u_prev, ReadData &reader, float stepSize);
		static WMpoint RungeKutta(WMpoint u_prev, ReadData &reader, float stepSize);
		
	private:
		static bool findVectorDataForPoint(WMpoint dataPoint, vecData &returnVec, ReadData &reader);
		static vecData interpolateVectorData(int x1, int x2, int y1, int y2, int x, int y,
											 ReadData &reader);
		static vecData BilinearInterpolation(float x1, float x2, float y1, float y2, float x, float y, ReadData &reader);

};


#endif
