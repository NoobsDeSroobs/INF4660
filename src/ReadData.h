#include <string>
#include <H5Cpp.h>
#include <math.h>
using std::string;

struct vector{
	float x;
	float y;
	bool trueData;

	vector(float x, float y): x(x), y(y), trueData(false){

	}

	float length(){
		return sqrt(pow(x, 2)+pow(y, 2));
	}
};

class ReadData{
	unsigned int width;
	unsigned int height;
	public:
		ReadData();
		virtual ~ReadData();
		
		void readFromFile(string fileName, string groupName = "Velocity",
				string compXName = "X-comp", string compYName = "Y-comp");
		
		vector getVector(int x, int y);
		
		unsigned int getWidth(){return width;}
		unsigned int getHeight(){return height;}

	private:
		double** dataXComp;
		double** dataYComp;
		
};
