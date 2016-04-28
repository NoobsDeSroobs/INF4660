#pragma once
#include <string>
#include <H5Cpp.h>
#include <math.h>
#include <vector>
using std::vector;
#include "Integration.h"
using std::string;



class ReadData{
	int width;
	int height;
	
	public:
		ReadData();
		virtual ~ReadData();
		
		void readFromHDF5File(string fileName,  bool transpose = false, string groupName = "Velocity",
				string compXName = "X-comp", string compYName = "Y-comp");
		
		void readFromTextFile(string fileName, int rows, int columns, bool transpose = false);
		
		velVector getVector(int x, int y);
		
		int getWidth(){return width;}
		int getHeight(){return height;}

	private:
		vector<std::vector<float>> dataXComp;
		vector<std::vector<float>> dataYComp;
		
};
