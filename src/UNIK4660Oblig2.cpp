//============================================================================
// Name        : UNIK4660Oblig2.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <iostream>
#include <H5Cpp.h>
#include <string.h>
#include "SDLRenderer.h"
#include "ReadData.h"
using namespace std;


void Test(SDLRenderer &renderer){
	renderer.SetTexture("/home/noobsdesroobs/Downloads/arrow.bmp");
	renderer.renderImgAtPos(50, 50, 0, 0, 300, 10, 45);
	renderer.renderImgAtPos(300, 50, 0, 0, 300, 10, 100);
	renderer.renderToScreen();
}
void drawAllPoints(ReadData &data, SDLRenderer& renderer);

//This system is LITTLE ENDIAN!!!!
int main() {
	SDLRenderer renderer(1000, 1000);
	renderer.setupSDLWindow();
	renderer.SetTexture("/home/noobsdesroobs/Downloads/arrow.bmp");
	//Test(renderer);

	string isabellPath = "/home/noobsdesroobs/Downloads/isabel_2d.h5";
	string metsimPath = "/home/noobsdesroobs/Downloads/metsim1_2d.h5";

	ReadData isabellData;
	isabellData.readFromFile(isabellPath);
	ReadData metsimData;
	metsimData.readFromFile(metsimPath);

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
	    	if(event.type == SDL_RELEASED){
	    		continue;
	    	}
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
			  case SDLK_r:
				  drawAllPoints(metsimData, renderer);
				  break;

			  default:

				break;
	    	 }

	    }
	}

	renderer.killSDL();
	return 0;
}

void drawAllPoints(ReadData &data, SDLRenderer& renderer){
	float xPixelStep = renderer.SCREEN_WIDTH/(float)data.getWidth();
	float yPixelStep = renderer.SCREEN_HEIGHT/(float)data.getHeight();
	vector baseVec(1, 0);
	renderer.clear();
	int ctr = 1;
	float rad2deg = 180/M_PI;
	//For each point. point.render()
	for(unsigned int i = 0; i < renderer.SCREEN_HEIGHT; i = i + 10){
		for(unsigned int k = 0; k < renderer.SCREEN_WIDTH; k = k + 10){
//			ctr++;
//			vector currentVec = data.getVector(k, i);
//			float vecLen = currentVec.length();
//			if(vecLen == 0.0f){
//				continue;
//			}
//			vector normedVec(currentVec.x/vecLen, normedVec.y/vecLen);
//			double angle = acos(normedVec.x);
//			angle = angle*rad2deg;
//			float pixelCord = k*xPixelStep;
//			float piyelCord = i*yPixelStep;
			cout << xPixelStep << " , " << yPixelStep << " , " << ctr << endl;
			renderer.PutPixel32_nolock(renderer.getMainSurface(), i+1, k+1, renderer.RGBA2INT(255, 0, 0, 255));
			renderer.PutPixel32_nolock(renderer.getMainSurface(), i, k+1, renderer.RGBA2INT(255, 0, 0, 255));
			renderer.PutPixel32_nolock(renderer.getMainSurface(), i+1, k, renderer.RGBA2INT(255, 0, 0, 255));
			renderer.PutPixel32_nolock(renderer.getMainSurface(), i, k, renderer.RGBA2INT(255, 0, 0, 255));
//			int ret = renderer.renderImgAtPos((int)pixelCord, (int)piyelCord, 0, 0, 20, 20, 0);
//			if (ret == -1){
//				return;
//			}
		}
	}
	renderer.renderToScreen();
}
