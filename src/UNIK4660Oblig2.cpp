//============================================================================
// Name        : UNIK4660Oblig2.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <H5Cpp.h>
#include <vector>
#include <string.h>
#include <bitset>
using namespace std;

int main() {

	H5::H5File file ("/home/noobsdesroobs/Downloads/metsim1_2d.h5", H5F_ACC_RDONLY);

	H5::Group velocity = file.openGroup("Velocity");
	H5::DataSet velXComp = velocity.openDataSet("X-comp");
	H5::DataSet velYComp = velocity.openDataSet("Y-comp");
	H5::DataSpace space = velXComp.getSpace();
	H5::DataType type = velXComp.getDataType();

	int rank;
	hsize_t dims[2];
	rank = space.getSimpleExtentDims(dims);

	H5::DataSpace universe(2, dims);

	float data_out[500][500];
	velXComp.read(data_out, H5::PredType::NATIVE_FLOAT, universe, space);

	int size = 500;

	cout << "Size of type: " << type.getSize() << endl <<
			"Num dims: " << space.getSimpleExtentNdims() << endl <<
			"Size of dims: " << space.getSelectNpoints() << endl;

	for(int i = 0; i < dims[0]; i++){
		for(int j = 0; j < dims[1]; j++){
			float f = data_out[i][j];
			if(6.013455 == f)
			cout << "Value: " << f << endl;
		}
	}

	file.close();

	return 0;
}


