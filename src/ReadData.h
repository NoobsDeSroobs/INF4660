#include <string>
#include <H5Cpp.h>
#include <math.h>
#include <vector>
#include "Integration.h"
using std::string;



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
