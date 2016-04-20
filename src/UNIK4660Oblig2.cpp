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
#include <SDL.h>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* screenSurface = NULL;

void PutPixel32_nolock(SDL_Surface * surface, int x, int y, Uint32 color)
{
    Uint8 * pixel = (Uint8*)surface->pixels;
    pixel += (y * surface->pitch) + (x * sizeof(Uint32));
    *((Uint32*)pixel) = color;
}

void setupSDLWindow(){

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}else{
		//Create window
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		} else {
			//Get window surface
			screenSurface = SDL_GetWindowSurface( window );

			//Fill the surface white
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );

			for(int i = 0; i < 10; i++){
				for (int j = 0; j < 10; ++j) {
					PutPixel32_nolock(screenSurface, 10+i, 10+j, (Uint32)123);
				}
			}

			//Update the surface
			SDL_UpdateWindowSurface( window );

			//Wait two seconds
			SDL_Delay( 2000 );
		}
	}

}

void killSDL(){

    //Destroy window
    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    SDL_Quit();

}



//This system is LITTLE ENDIAN!!!!
int main() {

	setupSDLWindow();
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
	killSDL();
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

