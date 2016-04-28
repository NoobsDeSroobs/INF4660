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
#include <ctime>
#include <unistd.h>
#include <string>
using namespace std;

void drawAllArrows(ReadData &data, SDLRenderer& renderer);
void drawStreamLines(ReadData &data, SDLRenderer& renderer);
float convolutionStartPoint(Streamline &stream, float weightLUT, 
			vector<vector<int> > &contributors, vector<vector<float> > const &noiseTexture,
			vector<vector<float> > &outputImage);
void convolutionFwdAndBwd(float startI, Streamline stream, float weightLUT,
			vector<vector<int> > &contributors, vector<vector<float> > const &noiseTexture,
			vector<vector<float> > &outputImage);
void doLICLoop(ReadData &dataset, SDLRenderer &renderer, int squareRes, int streamLength, 
			   bool bidirectional, float stepSize, INTEGRATION_METHOD inter);

const static string imgFileName = "M10x10RK500S.bmp";

void paintCriticalTOBlack(ReadData &data, SDLRenderer &renderer){
	float dataToPixel = renderer.SCREEN_HEIGHT/data.getHeight();
	for (int x = 0; x < data.getWidth(); ++x) {
		for (int y = 0; y < data.getHeight(); ++y) {
			if(data.getVector(x, y).length() == 0){
				renderer.PutPixel32_nolock(renderer.getMainSurface(), x*dataToPixel, y*dataToPixel, renderer.RGBA2INT(0, 0, 0, 255));
				renderer.PutPixel32_nolock(renderer.getMainSurface(), (x*dataToPixel)+1, (y*dataToPixel)+1, renderer.RGBA2INT(0, 0, 0, 255));
			}
		}
	}

}

void calculate10X10StreamLine(ReadData &data, SDLRenderer &renderer){
	int length = 500;

	//282, 382

	int stride = data.getHeight()/10;
	float dataToPixelCoord = renderer.SCREEN_HEIGHT/(float)data.getHeight();
	for (int y = 0; y < data.getHeight(); y = y + stride) {
		for (int x = 0; x < data.getHeight(); x = x + stride) {
			Streamline stream(x, y, length, false, 0.25, RK, data);
			vector<WMpoint> curve = stream.getCurvePoints();
			fprintf(stderr, "Seed x: %d, y: %d\n", x, y);
			if(curve.size() > 10 && !stream.isCriticalPoint()){
				for (uint var = 0; var < curve.size(); ++var) {
					//fprintf(stderr, "CurvePoint %d: <%f, %f>\n", var, curve[var].x, curve[var].y);
					curve[var].x = curve[var].x*dataToPixelCoord;
					curve[var].y = curve[var].y*dataToPixelCoord;
				}
				renderer.drawLine(curve);
			}
		}
	}
}

void calculateRandomStreamLine(ReadData &data, SDLRenderer &renderer){
	int length = 500;
	float dataToPixelCoord = renderer.SCREEN_HEIGHT/(float)data.getHeight();
	
	//282, 382
	
	int counter = 0;
	while(counter < 50){
		counter++;
		float x = rand();
		x = (int)x%(data.getHeight()*1000);
		x = x / 1000;
		float y = rand();
		y = (int)y%(data.getHeight()*1000);
		y = y / 1000;

		Streamline stream(x, y, length, false, 0.25, RK, data);
		vector<WMpoint> curve = stream.getCurvePoints();
		fprintf(stderr, "Seed x: %d, y: %d\n", x, y);
		if(curve.size() > 10 && !stream.isCriticalPoint()){
			for (uint var = 0; var < curve.size(); ++var) {
				//fprintf(stderr, "CurvePoint %d: <%f, %f>\n", var, curve[var].x, curve[var].y);
				curve[var].x = curve[var].x*dataToPixelCoord;
				curve[var].y = curve[var].y*dataToPixelCoord;
			}
			renderer.drawLine(curve);
		}
	}
}

void completeRun(ReadData &dataset1, ReadData &dataset2, SDLRenderer &renderer, int squareRes, int streamLength, 
				 bool bidirectional, float stepSize, INTEGRATION_METHOD inter){
	clock_t begin_time;
	//squareRes has to be set in main()
	//------------------------------------
	fprintf(stderr, "Starting on Isabel");
	//Run1 - Isabel, Euler, dynamic length
	streamLength = squareRes * 0.20;
	inter = EULER;
	renderer.clear(); 
	
	begin_time = clock();
	doLICLoop(dataset1, renderer, squareRes, streamLength, bidirectional, stepSize, inter);
	float timeSpent = float(clock() - begin_time) / CLOCKS_PER_SEC;
	string fileName = "Isabel-Euler_DL_" + to_string(squareRes) + "_" + to_string(timeSpent) + ".bmp";
	SDL_SaveBMP(renderer.getMainSurface(), fileName.c_str());
	
	fprintf(stderr, "Run 1, Size %d\n", squareRes);
	//------------------------------------
	//Run2 - Isabel, RK, dynamic length
	streamLength = squareRes * 0.20;
	inter = RK;
	renderer.clear(); 
	
	begin_time = clock();
	doLICLoop(dataset1, renderer, squareRes, streamLength, bidirectional, stepSize, inter);
	timeSpent = float(clock() - begin_time) / CLOCKS_PER_SEC;
	fileName = "Isabel-RK_DL_" + to_string(squareRes) + "_" + to_string(timeSpent) + ".bmp";
	SDL_SaveBMP(renderer.getMainSurface(), fileName.c_str());
	
	fprintf(stderr, "Run 2, Size %d\n", squareRes);
	//------------------------------------
	//Run3 - Isabel, RK, length 100
	streamLength = 100;
	inter = RK;
	renderer.clear(); 
	
	begin_time = clock();
	doLICLoop(dataset1, renderer, squareRes, streamLength, bidirectional, stepSize, inter);
	timeSpent = float(clock() - begin_time) / CLOCKS_PER_SEC;
	fileName = "Isabel-RK_100L_" + to_string(squareRes) + "_" + to_string(timeSpent) + ".bmp";
	SDL_SaveBMP(renderer.getMainSurface(), fileName.c_str());
	
	fprintf(stderr, "Run 3, Size %d\n", squareRes);
	//------------------------------------
	//Run4 - Isabel, RK, length 400
	streamLength = 100;
	inter = RK;
	renderer.clear(); 
	
	begin_time = clock();
	doLICLoop(dataset1, renderer, squareRes, streamLength, bidirectional, stepSize, inter);
	timeSpent = float(clock() - begin_time) / CLOCKS_PER_SEC;
	fileName = "Isabel-RK_400L_" + to_string(squareRes) + "_" + to_string(timeSpent) + ".bmp";
	SDL_SaveBMP(renderer.getMainSurface(), fileName.c_str());
	
	fprintf(stderr, "Run 4, Size %d\n", squareRes);
	
	
	
	
	fprintf(stderr, "Starting on Metsim");
	//------------------------------------
	//Run5 - Metsim, Euler, dynamic length
	streamLength = squareRes * 0.20;
	inter = EULER;
	renderer.clear(); 
	
	begin_time = clock();
	doLICLoop(dataset2, renderer, squareRes, streamLength, bidirectional, stepSize, inter);
	timeSpent = float(clock() - begin_time) / CLOCKS_PER_SEC;
	fileName = "Metsim-Euler_DL_" + to_string(squareRes) + "_" + to_string(timeSpent) + ".bmp";
	SDL_SaveBMP(renderer.getMainSurface(), fileName.c_str());
	fprintf(stderr, "Run 5, Size %d\n", squareRes);
	
	//------------------------------------
	//Run6 - Metsim, RK, dynamic length
	streamLength = squareRes * 0.20;
	inter = RK;
	renderer.clear(); 
	
	begin_time = clock();
	doLICLoop(dataset2, renderer, squareRes, streamLength, bidirectional, stepSize, inter);
	timeSpent = float(clock() - begin_time) / CLOCKS_PER_SEC;
	fileName = "Metsim-RK_DL_" + to_string(squareRes) + "_" + to_string(timeSpent) + ".bmp";
	SDL_SaveBMP(renderer.getMainSurface(), fileName.c_str());
	
	fprintf(stderr, "Run 6, Size %d\n", squareRes);
	//------------------------------------
	//Run7 - Metsim, RK, length 100
	streamLength = squareRes * 0.20;
	inter = RK;
	renderer.clear(); 
	
	begin_time = clock();
	doLICLoop(dataset2, renderer, squareRes, streamLength, bidirectional, stepSize, inter);
	timeSpent = float(clock() - begin_time) / CLOCKS_PER_SEC;
	fileName = "Metsim-RK_100L_" + to_string(squareRes) + "_" + to_string(timeSpent) + ".bmp";
	SDL_SaveBMP(renderer.getMainSurface(), fileName.c_str());
	
	fprintf(stderr, "Run 7, Size %d\n", squareRes);
	//------------------------------------
	//Run8 - Metsim, RK, length 400
	streamLength = squareRes * 0.20;
	inter = RK;
	renderer.clear(); 
	
	begin_time = clock();
	doLICLoop(dataset2, renderer, squareRes, streamLength, bidirectional, stepSize, inter);
	timeSpent = float(clock() - begin_time) / CLOCKS_PER_SEC;
	fileName = "Metsim-RK_400L_" + to_string(squareRes) + "_" + to_string(timeSpent) + ".bmp";
	SDL_SaveBMP(renderer.getMainSurface(), fileName.c_str());
	
	fprintf(stderr, "Run 8, Size %d\n", squareRes);
}

//This system is LITTLE ENDIAN!!!!
int main() {
	//Run variables:
	int squareRes = 1000;
	int streamLength = squareRes * 0.20; //Fast LIC supposed to used 1/10 of 2 * resolution length
	bool bidirectional = true;
	float stepSize = 0.25f;
	INTEGRATION_METHOD inter = RK;
	
	SDLRenderer renderer(squareRes, squareRes);
	renderer.setupSDLWindow();
	renderer.SetTexture("arrow.bmp");

	string isabelPath = "../Data/isabel_2d.h5";
	string metsimPath = "../Data/metsim1_2d.h5";
	
	string asciiNameIsabel = "../Data/ascii_isabel";
	string asciiNameMetsim = "../Data/ascii_metsim";

	ReadData isabelData;
	//isabelData.readFromHDF5File(isabelPath, true);
	isabelData.readFromTextFile(asciiNameIsabel, 500, 500, true);
	ReadData metsimData;
	//metsimData.readFromHDF5File(metsimPath, false);
	metsimData.readFromTextFile(asciiNameMetsim, 127, 127);
	
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
			if(event.type == SDL_KEYDOWN){
				switch( event.key.keysym.sym ){

				case SDLK_UP:
					  paintCriticalTOBlack(metsimData, renderer);
					calculateRandomStreamLine(metsimData, renderer);
					//drawStreamLines(isabellData, noiseImageRenderer);
					//noiseImageRenderer.renderToScreen();
					break;
				  case SDLK_DOWN:
					  paintCriticalTOBlack(isabelData, renderer);
					calculate10X10StreamLine(isabelData, renderer);
					//drawStreamLines(isabellData, noiseImageRenderer);
					//noiseImageRenderer.renderToScreen();
					break;
					
					case SDLK_ESCAPE:
						/* Quit the application */
						RUNNING = false;
						break;
						
					case SDLK_r:
						drawAllArrows(isabelData, renderer);
						break;
					
					case SDLK_d:
						renderer.renderToScreen();
						SDL_SaveBMP(renderer.getMainSurface(), imgFileName.c_str());
						break;
					
					case SDLK_c:
						renderer.clear();
						renderer.renderToScreen();
						break;
					
					case SDLK_i:
						renderer.clear();
						doLICLoop(isabelData, renderer, squareRes, streamLength, bidirectional,
								  stepSize, inter);
						renderer.renderToScreen();
						SDL_SaveBMP(renderer.getMainSurface(), "Isabel.bmp");
						break;
						
					case SDLK_m:
						renderer.clear();
						doLICLoop(metsimData, renderer, squareRes, streamLength, bidirectional,
								  stepSize, inter);
						renderer.renderToScreen();
						SDL_SaveBMP(renderer.getMainSurface(), "Metsim.bmp");
						break;
					
					case SDLK_q:
						completeRun(isabelData, metsimData, renderer, squareRes, 
									streamLength, bidirectional, stepSize, inter);
						break;

					default:
						break;
				 }
			}
		}
	}
	fprintf(stderr, "Finished with the program.");
	renderer.killSDL();
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
	for(int i = 0; i < data.getHeight(); i = i + 5){
		for(int k = 0; k < data.getWidth(); k = k + 5){
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
}

bool isIsolated(WMpoint seed, float d, Streamline &csl){

		std::vector<WMpoint> SLPoints = csl.getCurvePoints();
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
	std::queue<WMpoint> seedCandidates;
	std::vector<Streamline> finishedSLs;
	std::queue<Streamline> SLQueue;

	//Build candidates
	for (int y = 0; y < data.getHeight(); y = y + stride) {
		for (int x = 0; x < data.getWidth(); x = x + stride) {
			seedCandidates.push(WMpoint(x, y));
		}
	}

	WMpoint candidateSeed = seedCandidates.front();
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
		std::vector<WMpoint> curve = sl.getCurvePoints();
		for (uint pointIter = 0; pointIter < curve.size()-1; ++pointIter) {
			renderer.drawLine(curve);
		}
	}

}

//Creates the input noise image texture
void createNoiseTexture(int width, int height, vector<vector<float>> &img){
	float greyscale;
	
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			greyscale = ((float) rand() / RAND_MAX);
			
			//No idea what is correct here!
			img[i][j] = greyscale;
		}
	}
}

//Returns the intensity of the pixel that is being convoluted
float convolutionStartPoint(Streamline &stream, float weightLUT, 
						   vector<vector<int> > &contributors, 
						   vector<vector<float> > const &noiseTexture,
						   vector<vector<float> > &outputImage, float dataToPixel){
	
	float weightAccumulator = 0.0;
	float texAccumulator = 0.0;
	float pointWeight = weightLUT;
	//Build accumulators
	for(WMpoint p: stream.getCurvePoints()){
		weightAccumulator += pointWeight; 
		texAccumulator += noiseTexture[p.x*dataToPixel][p.y*dataToPixel] * pointWeight;
	}
	
	//Set intensity
	float intensity = texAccumulator / weightAccumulator; //Averages it
	WMpoint p = stream.getStartPoint();
	outputImage[p.x*dataToPixel][p.y*dataToPixel] += intensity;
	contributors[p.x*dataToPixel][p.y*dataToPixel]++;
	
	return intensity;
}
void convolutionFwdAndBwd(float startI, Streamline stream, float weightLUT,
						  vector<vector<int> > &contributors, 
						  vector<vector<float> > const &noiseTexture,
						  vector<vector<float> > &outputImage, float dataToPixel){
	float prevI;
	WMpoint streamStart = stream.getStartPoint();
	vector<WMpoint> streamForward = stream.getCurveForwardPoints();
	vector<WMpoint> streamBackwards = stream.getCurveBackwardPoints();
	WMpoint p, pLeapFwdN, pLeapBwdN;
	
	//Find max M, half of shortest length
	int minLength = (streamForward.size() < streamBackwards.size())? streamForward.size() : streamBackwards.size();
	int M = minLength / 2;
	double weight = 1.0 /minLength * 2;
	
	//Forwards
	prevI = startI;
	for(int m1 = 0; m1 < M; m1++){
		p = streamForward[m1];
		pLeapFwdN = streamForward[m1+M];
		pLeapBwdN = (M-1 > m1) ? streamBackwards[M-1 - m1] : streamStart;
		
		//I(Xm+1) = I(Xm) + k*( T(Xm+1+n) - T(Xm-n) )
		float intensity = prevI + weight * (noiseTexture[pLeapFwdN.x*dataToPixel][pLeapFwdN.y*dataToPixel] -
											noiseTexture[pLeapBwdN.x*dataToPixel][pLeapBwdN.y*dataToPixel]);
		
		if(contributors[p.x*dataToPixel][p.y*dataToPixel] > 10){
			outputImage[p.x*dataToPixel][p.y*dataToPixel] += intensity;
			contributors[p.x*dataToPixel][p.y*dataToPixel]++; //Should expect this is needed?
		}
		prevI = intensity;
	}
	
	//Backwards
	prevI = startI;
	for(int m1 = 0; m1 < M; m1++){
		p = streamBackwards[m1];
		pLeapBwdN = streamBackwards[m1+M];
		pLeapFwdN = (M-1 > m1) ? streamForward[M-1 - m1] : streamStart;
		
		//I(Xm-1) = I(Xm) + k*( T(Xm-1-n) - T(Xm+n) )
		float intensity = prevI + weight * (noiseTexture[pLeapBwdN.x*dataToPixel][pLeapBwdN.y*dataToPixel] +
											noiseTexture[pLeapFwdN.x*dataToPixel][pLeapFwdN.y*dataToPixel]);
		
		if(contributors[p.x*dataToPixel][p.y*dataToPixel] > 10){
			outputImage[p.x*dataToPixel][p.y*dataToPixel] += intensity;
			contributors[p.x*dataToPixel][p.y*dataToPixel]++; //Should expect this is needed?
		}
		prevI = intensity;
	}
}
//Loop for LIC

void doLICLoop(ReadData &dataset, SDLRenderer &renderer, int squareRes, int streamLength, 
			   bool bidirectional, float stepSize, INTEGRATION_METHOD inter){
	//Create noise and output image
	vector<vector<float> > outputImage(squareRes, vector<float>(squareRes, 0));
	vector<vector<float> > noiseTexture(squareRes, vector<float>(squareRes, 0));
	createNoiseTexture(squareRes, squareRes, noiseTexture);
	
	//Variables
	float weightLUT = 1; //We only use a simple box filter for now
	float pixelToData = (float)dataset.getHeight()/(float)renderer.SCREEN_HEIGHT;
	float dataToPixel = (float)renderer.SCREEN_HEIGHT/(float)dataset.getHeight();
	stepSize = stepSize * pixelToData;
	
	//Fast LIC counters
	int maxContributors = 10;
	vector<vector<int> > contributors(squareRes, vector<int>(squareRes, 0));
	//Loop
	for(int x = 0; x < squareRes; x++){
		//fprintf(stderr, "x:%d\n", x);
		for(int y = 0; y < squareRes; y++){
			if(contributors[x][y] >= maxContributors){
				continue;
			}
			
			//Find streamline for current point
			Streamline stream(x*pixelToData, y*pixelToData, streamLength, bidirectional, stepSize, inter, dataset);
			if(stream.getCurvePoints().size() <= 1){
				outputImage[x][y] = 0;
				//contributors[x][y] = maxContributors;
				continue;
			}
			
			//Fast LIC, convolves both for current, backwards and forwards
			//First start point of the streamline, uses the entire streamline
			float intensity = convolutionStartPoint(stream, weightLUT, contributors, 
													noiseTexture, outputImage, dataToPixel);
				
			//Then forwards and backwards, only halfway up and down
			convolutionFwdAndBwd(intensity, stream, weightLUT, contributors, noiseTexture, outputImage, dataToPixel);
		}
	}
		
	//Normalize the values and paint
	for(int x = 0; x < squareRes; x++){
		for(int y = 0; y < squareRes; y++){
			outputImage[x][y] /= contributors[x][y];
			
			int c = (int) (outputImage[x][y]*255);
			renderer.PutPixel32_nolock(renderer.getMainSurface(), x, y, renderer.RGBA2INT(c,c,c,255));
		}
	}
}


