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
#include "Streamline.h"
#include <cmath>
#include <queue>

using namespace std;


void Test(SDLRenderer &renderer){
	renderer.SetTexture("/home/noobsdesroobs/Downloads/arrow.bmp");
	renderer.renderImgAtPos(50, 50, 0, 0, 300, 10, 45);
	renderer.renderImgAtPos(300, 50, 0, 0, 300, 10, 100);
	renderer.renderToScreen();
}
void drawAllArrows(ReadData &data, SDLRenderer& renderer);
void drawStreamLines(ReadData &data, SDLRenderer& renderer);
void drawLic(ReadData &data, SDLRenderer& renderer);

//This system is LITTLE ENDIAN!!!!
int main() {
	SDLRenderer renderer(500, 500);
	renderer.setupSDLWindow();
	SDLRenderer noiseImageRenderer(renderer.SCREEN_WIDTH, renderer.SCREEN_HEIGHT);
	noiseImageRenderer.setupSDLWindow("Noise Image", true);
	renderer.SetTexture("/home/noobsdesroobs/Downloads/arrow.bmp");
	//Test(renderer);

	string isabellPath = "/home/noobsdesroobs/Downloads/isabel_2d.h5";
	string metsimPath = "/home/noobsdesroobs/Downloads/metsim1_2d.h5";

	ReadData isabellData;
	isabellData.readFromFile(isabellPath, true);
	ReadData metsimData;
	metsimData.readFromFile(metsimPath, false);

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
				drawLic(isabellData, noiseImageRenderer);
				noiseImageRenderer.renderToScreen();

				break;

			  case SDLK_UP:
				printf( "Move time backwards.\n" );
				break;

			  case SDLK_ESCAPE:
				/* Quit the application */
				RUNNING = false;
				break;
			  case SDLK_r:
				  drawAllArrows(isabellData, renderer);
				  break;

			  default:

				break;
	    	 }

	    }
	}
	fprintf(stderr, "Finished with the program.");
	renderer.killSDL();
	noiseImageRenderer.killSDL();
	return 0;
}

void drawAllArrows(ReadData &data, SDLRenderer& renderer){
	float xPixelStep = renderer.SCREEN_WIDTH/(float)data.getWidth();
	float yPixelStep = renderer.SCREEN_HEIGHT/(float)data.getHeight();
	velVector baseVec(1.0f, 0.0f);
	renderer.clear();
	int ctr = 1;
	float rad2deg = 180/M_PI;
	//For each point. point.render()
	for(unsigned int i = 0; i < data.getHeight(); i = i + 10){
		for(unsigned int k = 0; k < data.getWidth(); k = k + 10){
			ctr++;
			velVector currentVec = data.getVector(k, i);
			float vecLen = currentVec.length();
			if(vecLen == 0.0f){
				continue;
			}
			velVector normedVec(currentVec.x/vecLen, normedVec.y/vecLen);
			double angle = acos(normedVec.x);
			angle = angle*rad2deg;
			float pixelCord = k*xPixelStep;
			float piyelCord = i*yPixelStep;
//			renderer.PutPixel32_nolock(renderer.getMainSurface(), i+1, k+1, renderer.RGBA2INT(255, 0, 0, 255));
//			renderer.PutPixel32_nolock(renderer.getMainSurface(), i, k+1, renderer.RGBA2INT(255, 0, 0, 255));
//			renderer.PutPixel32_nolock(renderer.getMainSurface(), i+1, k, renderer.RGBA2INT(255, 0, 0, 255));
//			renderer.PutPixel32_nolock(renderer.getMainSurface(), i, k, renderer.RGBA2INT(255, 0, 0, 255));
			int ret = renderer.renderImgAtPos((int)pixelCord, (int)piyelCord, 0, 0, 10, 5, angle);
			if (ret == -1){
				fprintf(stderr, "Failed to draw picture: %d.\n", ctr);
				return;
			}
		}
	}
	renderer.renderToScreen();
	SDL_SaveBMP(renderer.getMainSurface(), "Isabel.bmp");
}

bool isIsolated(point seed, float d, Streamline &csl){

		std::vector<point> SLPoints = csl.getCurvePoints();
		for (uint k = 0; k < SLPoints.size(); ++k) {
			if(seed.distanceTo(SLPoints[k]) < d){
				return false;
			}
		}

	return true;
}

void drawStreamLines(ReadData &data, SDLRenderer& renderer){
	float stepSize = 0.25f;
	float d = 1;
	int stride = 10;
	int length = 6;
	std::queue<point> seedCandidates;
	std::vector<Streamline> finishedSLs;
	std::queue<Streamline> SLQueue;

	//Build candidates
	for (uint y = 0; y < data.getHeight(); y = y + stride) {
		for (uint x = 0; x < data.getWidth(); x = x + stride) {
			seedCandidates.push(point(x, y));
		}
	}

	point candidateSeed = seedCandidates.front();
	seedCandidates.pop();
	//Compute an initial streamline and make it current.
	//Queue streamline Queue
	bool finished = false;
	bool validCandidate = false;

	Streamline currentStreamLine(candidateSeed.x, candidateSeed.y, length, false, stepSize, 0, data);
	finishedSLs.push_back(currentStreamLine);
	while(!finished){
		while(!validCandidate){
			if(seedCandidates.size() > 0){
				finished = true;
				break;
			}
			//Select a seedpoint with minimum d distance to current streamline
			candidateSeed = seedCandidates.front();
			seedCandidates.pop();
			if(isIsolated(candidateSeed, d, currentStreamLine)){
				validCandidate = true;
			}
		}

		if(validCandidate){
			//Compute streamline and add to streamline queue.
			SLQueue.push(Streamline(candidateSeed.x, candidateSeed.y, length, false, stepSize, 0, data));
			validCandidate = false;
		}else{
			if(SLQueue.size() == 0){
				finished = true;
			}else{
				currentStreamLine = SLQueue.front();
				finishedSLs.push_back(currentStreamLine);
			}
		}
	}


	for (uint slIter = 0; slIter < finishedSLs.size(); ++slIter) {
		Streamline sl = finishedSLs[slIter];
		std::vector<point> curve = sl.getCurvePoints();
		for (uint pointIter = 0; pointIter < curve.size()-1; ++pointIter) {
			renderer.drawLine(curve);
		}
	}

}


void drawLic(ReadData &data, SDLRenderer& noiseImageRenderer){

	int grey = rand();
	for (int y = 0; y < noiseImageRenderer.SCREEN_HEIGHT; ++y) {
		for (int x = 0; x < noiseImageRenderer.SCREEN_WIDTH; ++x) {
			noiseImageRenderer.PutPixel32_nolock(noiseImageRenderer.getMainSurface(), x, y,
					noiseImageRenderer.RGBA2INT(grey, grey, grey, grey));
			grey = rand();
		}
	}


}
