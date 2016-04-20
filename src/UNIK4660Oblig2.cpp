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
	renderer.SetTexture("/home/noobsdesroobs/Downloads/arrow.bmp");
	renderer.renderImgAtPos(50, 50, 0, 0, 300, 10, 45);
	renderer.renderImgAtPos(300, 50, 0, 0, 300, 10, 100);
	renderer.renderToScreen();
}
void drawAllPoints();

//This system is LITTLE ENDIAN!!!!
int main() {
	SDLRenderer renderer(680, 480);
	renderer.setupSDLWindow();
	Test(renderer);
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

	file.close();


	bool RUNNING = true;
	/* An SDL_Event */
    SDL_Event event;
	while(RUNNING)
	{
	    while(SDL_PollEvent(&event))
	    {

	    	if(event.type == SDL_QUIT){
	    		RUNNING = false;
	    	}

	    	/* If a quit event has been sent */
	    	switch( event.key.keysym.sym ){
			  case SDLK_DOWN:
				printf( "Move time forward.\n" );
				break;

			  case SDLK_UP:
				printf( "Move time backwards.\n" );
				break;

			  case SDLK_ESCAPE:
				/* Quit the application */
				RUNNING = false;
				break;

			  default:
				  drawAllPoints();
				break;
	    	 }

	    }
	}

	renderer.killSDL();
	return 0;
}

void drawAllPoints(){
	//For each point. point.render()
}
