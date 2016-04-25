/*
 * SDLRenderer.h
 *
 *  Created on: Apr 20, 2016
 *      Author: noobsdesroobs
 */

#ifndef SDLRENDERER_H_
#define SDLRENDERER_H_
#include <SDL.h>
#include <string>
#include <iostream>
#include <vector>
#include "Integration.h"
using namespace std;

class SDLRenderer{
public:
int SCREEN_WIDTH;
int SCREEN_HEIGHT;
private:
Uint32 GREENSCREEN;
//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* screenSurface = NULL;
SDL_Renderer* Main_Renderer = NULL;
SDL_Texture* Tex = NULL;

public:
SDLRenderer(int wWidth, int wHeight){
	SCREEN_WIDTH = wWidth;
	SCREEN_HEIGHT = wHeight;
	GREENSCREEN = 16711935;
}

void PutPixel32_nolock(SDL_Surface * surface, int x, int y, Uint32 color);
void setupSDLWindow(string windName = "Main window", bool greyScale = false);
void killSDL();
int renderImgAtPos(int x, int y, int imgX, int imgY, int width, int height, double angle);
void drawLine(std::vector<point> &curve);
void SetTexture(std::string path);
void renderToScreen(){
	SDL_UpdateWindowSurface( window );
}
void clear(){
	SDL_RenderClear( Main_Renderer );
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
}

SDL_Surface* getMainSurface(){
	return screenSurface;
}

Uint32 RGBA2INT(unsigned int iR, unsigned int iG, unsigned int iB, unsigned int iA)
{

  return ((iA * 256 + iR) * 256 + iG) * 256 + iB;
}

void setSurface(vector<vector<float>> &surf){
	SDL_Surface* s = getMainSurface();
	for (uint var = 0; var < surf.size(); ++var) {
		for (uint var2 = 0; var2 < surf[0].size(); ++var2) {
			PutPixel32_nolock(s, var2, var, RGBA2INT(surf[var][var2], surf[var][var2], surf[var][var2], surf[var][var2]));
		}
	}
}

};

#endif /* SDLRENDERER_H_ */
