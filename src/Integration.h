#ifndef INTEGRATIONS
#define INTEGRATIONS

#include <cmath>
class ReadData;

struct WMpoint{
	float x;
	float y;
	bool isValid;
	
	WMpoint(){
		x = 0.0;
		y = 0.0;
		isValid = true;
	}
	
	WMpoint(float x, float y): x(x), y(y){
		isValid = true;
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
	bool isValid;
	
	vecData(){
		x = 0;
		y = 0;
		trueData = false;
		isValid = true;
	}
	vecData(float x, float y): x(x), y(y), trueData(false), isValid(true){
	
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
		float RKPartial (WMpoint u_prev, ReadData &reader, float stepSize);
		static bool findVectorDataForPoint(WMpoint dataPoint, vecData &returnVec, ReadData &reader);
		static vecData interpolateVectorData(int x1, int x2, int y1, int y2, int x, int y,
											 ReadData &reader);
		static bool BilinearInterpolation(vecData &returnVec, float x1, float x2, float y1, 
										  float y2, float x, float y, ReadData &reader);

};


#endif
