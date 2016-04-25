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
#include <vector>
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

//New methods, must be merged with the others
//SDL_Texture createNoiseTexture(int width, int height);
float* createWeightLUT(int size);
float convolution(point pixelPoint);
void doLICLoop();


//This system is LITTLE ENDIAN!!!!
int main() {
	SDLRenderer renderer(500, 500);
	renderer.setupSDLWindow();
	SDLRenderer noiseImageRenderer(renderer.SCREEN_WIDTH, renderer.SCREEN_HEIGHT);
	noiseImageRenderer.setupSDLWindow("Noise Image");
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

	Streamline currentStreamLine(candidateSeed.x, candidateSeed.y, length, false, stepSize, EULER, data);
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
			SLQueue.push(Streamline(candidateSeed.x, candidateSeed.y, length, false, stepSize, EULER, data));
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

//Creates the input noise image texture
void createNoiseImage(int width, int height){
	/*
	SDL_Renderer noiseTexture(widt, height);
	int grey;
	
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			grey = rand();
			
			//No idea what is correct here!
			noiseTexture.PutPixel32_nolock(noiseTexture.getMainSurface(), x, y,
					noiseTexture.RGBA2INT(grey, grey, grey, grey));
		}
	}
	 * */
}
//Returns the intensity of the pixel that is being convoluted
float convolutionStartPoint(Streamline &stream, float weightLUT, 
						   vector<vector<int> > &contributors, 
						   vector<vector<float> > const &noiseTexture,
						   vector<vector<float> > &outputImage){
	
	float weightAccumulator = 0.0;
	float texAccumulator = 0.0;
	float pointWeight = 1; //weightLUT[] //Ignore weight for now, just use average, 
											 //is this the same as a constant filter kernel
											 //which fast lic needs?
	//Build accumulators
	for(auto &p : stream.getCurvePoints()){
		weightAccumulator += pointWeight; 
		texAccumulator += noiseTexture[p.x][p.y] * pointWeight;
	}
	
	//Set intensity
	point p = stream.getStartPoint();
	float intensity = texAccumulator / weightAccumulator;
	outputImage[p.x][p.y] += intensity;
	contributors[p.x][p.y]++;
	
	return intensity;
}
void convolutionFwdAndBwd(float startI, Streamline stream, float weightLUT,
						  vector<vector<int> > &contributors, 
						  vector<vector<float> > const &noiseTexture,
						  vector<vector<float> > &outputImage){
	float prevI = startI;
	float pointWeight = 1.0; //Just a simple box function for now.
	point streamStart = stream.getStartPoint();
	vector<point> streamForward = stream.getCurveForwardPoints();
	vector<point> streamBackwards = stream.getCurveBackwardPoints();
	point p, prevPoint, pLeapFwdN, pLeapBwdN;
	
	//Find max M, half of shortest length
	int minLength = (streamForward.size() < streamBackwards.size()) 
					? streamForward.size() : streamBackwards.size();
	int M = minLength / 2;
	int n = M;
	
	//Forwards
	prevPoint = streamStart;
	for(int m = 0; m < M; m++){
		p = streamForward[m];
		pLeapFwdN = streamForward[m+n];
		pLeapBwdN = (n-1 > m) ? streamBackwards[n-1 - m] : streamStart;
		
		//I(Xm+1) = I(Xm) + k*( T(Xm+1+n) - T(Xm-n) )
		float intensity = prevI + pointWeight * (noiseTexture[pLeapFwdN.x][pLeapFwdN.y] - 
												 noiseTexture[pLeapBwdN.x][pLeapBwdN.y]);
		
		outputImage[p.x][p.y] += intensity;
		contributors[p.x][p.y]++; //Should expect this is needed?
		
		prevI = intensity;
		prevPoint = streamForward[m];
	}
	
	//Backwards
	prevPoint = streamStart;
	prevI = startI;
	for(int m = 0; m < M; m++){
		p = streamBackwards[m];
		pLeapBwdN = streamBackwards[m+n];
		pLeapFwdN = (n-1 > m) ? streamForward[n-1 - m] : streamStart;
		
		//I(Xm-1) = I(Xm) + k*( T(Xm-1-n) - T(Xm+n) )
		float intensity = prevI + pointWeight * (noiseTexture[pLeapBwdN.x][pLeapBwdN.y] - 
												 noiseTexture[pLeapFwdN.x][pLeapFwdN.y]);
		
		outputImage[p.x][p.y] += intensity;
		contributors[p.x][p.y]++; //Should expect this is needed?
		
		prevI = intensity;
		prevPoint = streamBackwards[m];
	}
}
//Loop for LIC
void doLICLoop(int dataset, int squareRes, int weightSize){
	//Read the vector data
	ReadData reader;
	string isabelPath = "/home/noobsdesroobs/Downloads/isabel_2d.h5";
	string metsimPath = "/home/noobsdesroobs/Downloads/metsim1_2d.h5";
	string vectorFieldFile = (dataset == 0) ? isabelPath : metsimPath;
	bool transpose = (dataset == 0) ? true : false;
	reader.readFromFile(vectorFieldFile, transpose);
	
	//Create noise and output image -TODO
		//createNoiseTexture(squareResolution, squareResolution);
	//outputImage = SDL_Texture(size, size); = 0 for all x,y
	vector<vector<float> > noiseTexture(squareRes, vector<float>(squareRes, 0));
	vector<vector<float> > outputImage(squareRes, vector<float>(squareRes, 0));
	
	//Variables
	int streamLength = squareRes * 0.20;
	float stepSize = 0.5f;
	bool bidirectional = true;
	
	//Create weight/kernel function
	float weightLUT = 1; //We only use a simple box filter for now
	
	//Fast LIC counter
	int maxContributors = 10;
	vector<vector<int> > contributors(squareRes, vector<int>(squareRes, 0));
	
	//Loop
	for(int x = 1; x <= squareRes; x++){
		for(int y = 1; y <= squareRes; y++){
			if(contributors[x][y] >= maxContributors){
				continue;
			}
			
			//Find streamline for current point
			Streamline stream(x, y, streamLength, bidirectional, stepSize, EULER, reader);
			if(stream.isCriticalPoint()) continue;
			
			//Fast LIC, convolves both for current, backwards and forwards
			//First start point of the streamline, uses the entire streamline
			float intensity = convolutionStartPoint(stream, weightLUT, contributors, 
				noiseTexture, outputImage);
			
			//Then forwards and backwards, only halfway up and down
			convolutionFwdAndBwd(intensity, stream, weightLUT, contributors, noiseTexture, 
				outputImage);
			}
		}
	
	//Normalize the value
	for(int x = 1; x <= squareRes; x++){
		for(int y = 1; y <= squareRes; y++){
			outputImage[x][y] /= contributors[x][y];
		}
	}
}


