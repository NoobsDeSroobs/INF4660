/*
 * SDLRenderer.h
 *
 *  Created on: Apr 20, 2016
 *      Author: noobsdesroobs
 */

#ifndef SDLRENDERER_H_
#define SDLRENDERER_H_
#include <SDL.h>

class SDLRenderer{
int SCREEN_WIDTH;
int SCREEN_HEIGHT;
//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* screenSurface = NULL;

public:
SDLRenderer(int wWidth, int wHeight){
	SCREEN_WIDTH = wWidth;
	SCREEN_HEIGHT = wHeight;
}

void PutPixel32_nolock(SDL_Surface * surface, int x, int y, Uint32 color);
void setupSDLWindow();
void killSDL();

Uint32 RGBA2INT(unsigned int iR, unsigned int iG, unsigned int iB, unsigned int iA)
{
  return ((iA * 256 + iR) * 256 + iG) * 256 + iB;
}

};

#endif /* SDLRENDERER_H_ */
