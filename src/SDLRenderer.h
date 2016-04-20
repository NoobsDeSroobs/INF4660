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
using namespace std;

class SDLRenderer{
int SCREEN_WIDTH;
int SCREEN_HEIGHT;
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
void setupSDLWindow();
void killSDL();
void renderImgAtPos(int x, int y, int imgX, int imgY, int width, int height, double angle);
void SetTexture(std::string path);
void renderToScreen(){SDL_UpdateWindowSurface( window );}
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

};

#endif /* SDLRENDERER_H_ */
