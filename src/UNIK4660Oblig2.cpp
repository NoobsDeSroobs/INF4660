//============================================================================
// Name        : UNIK4660Oblig2.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <iostream>
#include <H5Cpp.h>
#include <string.h>
#include "SDLRenderer.h"
using namespace std;


void Test(SDLRenderer &renderer){
	unsigned int test = renderer.RGBA2INT(15, 15, 15, 15);
	unsigned int check = 252645135;

	cout << "Test:" << test << ", check:" << check << endl;

}


//This system is LITTLE ENDIAN!!!!
int main() {


	SDLRenderer renderer(680, 480);
	Test(renderer);
	renderer.setupSDLWindow();
	H5::H5File file ("/home/noobsdesroobs/Downloads/metsim1_2d.h5", H5F_ACC_RDONLY);

	H5::Group velocity = file.openGroup("Velocity");
	H5::DataSet velXComp = velocity.openDataSet("X-comp");
	H5::DataSet velYComp = velocity.openDataSet("Y-comp");
	H5::DataSpace space = velXComp.getSpace();
	H5::DataType type = velXComp.getDataType();

	H5::FloatType typefloat = velXComp.getFloatType();
	H5std_string orderStr;
	H5T_order_t order = typefloat.getOrder(orderStr);

	cout << "order: " << order << ", Str:" << orderStr << endl;

	//int rank;
	hsize_t dims[2];
	space.getSimpleExtentDims(dims);

	H5::DataSpace universe(2, dims);

	float data_out[dims[0]][dims[1]];
	velXComp.read((void*)data_out, H5::PredType::NATIVE_FLOAT, universe, space);

	//int size = 500;

	cout << "Size of type: " << type.getSize() << endl <<
			"Num dims: " << space.getSimpleExtentNdims() << endl <<
			"Size of dims: " << space.getSelectNpoints() << endl;
int counter = 0;
	//for(uint i = 0; i < dims[0]; i++){
		//for(uint j = 0; j < dims[1]; j++){
			float f = data_out[0][0];
			if(f != 0)
				counter++;
			//if(f == 0.012566418)
			cout << "Value: " << f << endl;
		//}
	//}


	cout << counter;
	file.close();
	cin >> dims[0];
	renderer.killSDL();
	return 0;
}

float forwardEulerIntegration(void){

	return 0;
}

float fourthOrdRungeKuttaIntegration(void){

	return 0;
}

double f(double x, double y[], int i) {
	return (x - y[0]); /* derivative of first equation */
}


void euler(double x, double y[], double step)
{
	int N = 1;
	double s[N]; /* for euler */
	int i;
	{
		for (i = 0; i < N; i++) {
			s[i] = step * f(x, y, i);
		}
	}

	{
		for (i = 0; i < N; i++)
			y[i] += s[i];
	}
}

