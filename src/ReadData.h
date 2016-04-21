#include <string>
#include <H5Cpp.h>
#include <math.h>
#include <vector>
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
} typedef velVector;

class ReadData{
	unsigned int width;
	unsigned int height;
	public:
		ReadData();
		virtual ~ReadData();
		
		void readFromFile(string fileName,  bool transpose = false, string groupName = "Velocity",
				string compXName = "X-comp", string compYName = "Y-comp");
		
		velVector getVector(int x, int y);
		
		unsigned int getWidth(){return width;}
		unsigned int getHeight(){return height;}

	private:
		std::vector<std::vector<float>> dataXComp;
		std::vector<std::vector<float>> dataYComp;
		
};
