#include "ReadData.h"

#include <iostream>
using std::cout; using std::endl;

ReadData::ReadData(){}
ReadData::~ReadData(){}

void ReadData::readFromFile(string fileName, string groupName ,
							string compXName , string compYName ){
	
	cout << "Reading from file: " << fileName << endl;
	H5::H5File file (fileName, H5F_ACC_RDONLY);
	H5::Group velocity = file.openGroup(groupName);
	
	
	
	cout << "Reading from dataset: " << compXName << endl;
	H5::DataSet velXComp = velocity.openDataSet(compXName);
	H5::DataSpace spaceX = velXComp.getSpace();
	hsize_t dimsX[2];
	spaceX.getSimpleExtentDims(dimsX);
	H5::DataSpace universeX(2, dimsX);
	width = dimsX[0];
	height = dimsX[1];
	double data_outX[dimsX[0]][dimsX[1]];
	velXComp.read((void*)data_outX, H5::PredType::NATIVE_FLOAT, universeX, spaceX);
	
	
	
	cout << "Reading from dataset: " << compYName << endl;
	H5::DataSet velYComp = velocity.openDataSet(compYName);
	H5::DataSpace spaceY = velYComp.getSpace();
	hsize_t dimsY[2];
	spaceY.getSimpleExtentDims(dimsY);
	H5::DataSpace universeY(2, dimsY);
	
	double data_outY[dimsY[0]][dimsY[1]];
	velYComp.read((void*)data_outY, H5::PredType::NATIVE_FLOAT, universeY, spaceY);
	
	dataXComp = new double*[dimsX[0]];
	dataYComp = new double*[dimsY[0]];
	for (unsigned int i = 0; i < dimsX[0]; ++i) {
		dataXComp[i] = new double[dimsX[1]];
		for (unsigned int k = 0; k < dimsX[1]; ++k) {
			dataXComp[i][k] = data_outX[i][k];
		}
	}
	for (unsigned int i = 0; i < dimsY[0]; ++i) {
		dataYComp[i] = new double[dimsY[1]];
		for (unsigned int k = 0; k < dimsX[1]; ++k) {
				dataYComp[i][k] = data_outY[i][k];
		}
	}

	file.close();
}

vector ReadData::getVector(int x, int y){
	double xVel = dataXComp[x][y];
	double yVel = dataYComp[x][y];
	if(xVel != 0.0f && yVel != 0.0f)
	printf("Pos: %d, %d has values x: %f, y: %f. \n", x, y, xVel, yVel);
	vector returnVec(xVel, yVel);
	returnVec.trueData = true;
	return returnVec;
}
