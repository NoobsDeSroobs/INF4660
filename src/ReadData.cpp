#include "ReadData.h"

#include <iostream>
using std::cout; using std::endl;

ReadData::ReadData(){}
ReadData::~ReadData(){}

void ReadData::readFromFile(string fileName, string groupName = "Velocity", 
							string compXName = "X-comp", string compYName = "Y-comp"){
	
	cout << "Reading from file: " << fileName << endl;
	H5::H5File file (fileName, H5F_ACC_RDONLY);
	H5::Group velocity = file.openGroup(groupName);
	
	
	
	cout << "Reading from dataset: " << compXName << endl
	H5::DataSet velXComp = velocity.openDataSet(compXName);
	H5::DataSpace spaceX = velXComp.getSpace();
	hsize_t dimsX[2];
	space.getSimpleExtentDims(dimsX);
	H5::DataSpace universeX(2, dimsX);
	
	float data_outX[dimsX[0]][dimsX[1]];
	velXComp.read((void*)data_outX, H5::PredType::NATIVE_FLOAT, universeX, spaceX);
	
	
	
	cout << "Reading from dataset: " << compYName << endl
	H5::DataSet velYComp = velocity.openDataSet(compYName);
	H5::DataSpace spaceY = velYComp.getSpace();
	hsize_t dimsY[2];
	space.getSimpleExtentDims(dimsY);
	H5::DataSpace universeY(2, dimsY);
	
	float data_outY[dimsY[0]][dimsY[1]];
	velXComp.read((void*)data_outY, H5::PredType::NATIVE_FLOAT, universeY, spaceY);
	
	
	
	dataXComp = data_outX;
	dataYComp = data_outY;
	file.close();
}

vector ReadData::getVector(int x, int y){
	vector returnVec;
	
	returnVec.x = dataXComp[x][y];
	returnVec.y = dataYComp[x][y];
	
	returnVec.trueData = true;
	return returnVec;
}