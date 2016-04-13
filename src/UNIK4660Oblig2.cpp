//============================================================================
// Name        : UNIK4660Oblig2.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <H5Cpp.h>
using namespace std;

int main() {

	H5::H5File file ("/home/noobsdesroobs/Downloads/metsim1_2d.h5", H5F_ACC_RDONLY);

	std::cout << 6 << std::endl;

	file.close();

	return 0;
}
