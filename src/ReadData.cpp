#include "ReadData.h"

#include <iostream>
using std::cout; using std::endl;

ReadData::ReadData(){}
ReadData::~ReadData(){}

void ReadData::readFromFile(string fileName, bool transpose, string groupName ,
							string compXName , string compYName){
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
	float data_outX[dimsX[0]][dimsX[1]];
	velXComp.read((void*)data_outX, H5::PredType::NATIVE_FLOAT, universeX, spaceX);
	
	
	
	cout << "Reading from dataset: " << compYName << endl;
	H5::DataSet velYComp = velocity.openDataSet(compYName);
	H5::DataSpace spaceY = velYComp.getSpace();
	hsize_t dimsY[2];
	spaceY.getSimpleExtentDims(dimsY);
	H5::DataSpace universeY(2, dimsY);
	
	float data_outY[dimsY[0]][dimsY[1]];
	velYComp.read((void*)data_outY, H5::PredType::NATIVE_FLOAT, universeY, spaceY);
	
	dataXComp.resize(dimsX[0], std::vector<float>(dimsX[1]));
	for (unsigned int i = 0; i < dimsX[0]; ++i) {
		for (unsigned int k = 0; k < dimsX[1]; ++k) {
			if(transpose){
				dataXComp[k][i] = data_outY[i][k];
			}else{
				dataXComp[k][i] = data_outX[i][k];
			}
		}
	}


	dataYComp.resize(dimsY[0], std::vector<float>(dimsY[1]));
	for (unsigned int i = 0; i < dimsY[0]; ++i) {
		for (unsigned int k = 0; k < dimsY[1]; ++k) {
			if(transpose){
				dataYComp[k][i] = data_outX[i][k];
			}else{
				dataYComp[k][i] = data_outY[i][k];
			}
		}
	}


	file.close();
}

velVector ReadData::getVector(int x, int y){
	if(x < 0 || x >= 500 || y < 0 || y >= 500 ){
		fprintf(stderr, "Tried to access data %d, %d which is out of bounds\n", x, y);
	}
	float xVel = dataXComp[x][y];
	float yVel = dataYComp[x][y];
	velVector returnVec(xVel, yVel);
	returnVec.trueData = true;
	return returnVec;
}
