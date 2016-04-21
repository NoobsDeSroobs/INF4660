/*
 * SDLRenderer.cpp
 *
 *  Created on: Apr 20, 2016
 *      Author: noobsdesroobs
 */
#include "SDLRenderer.h"


void SDLRenderer::PutPixel32_nolock(SDL_Surface * surface, int x, int y, Uint32 color)
{
	if(x > 0 && x < SCREEN_WIDTH && y > 0 && y < SCREEN_HEIGHT){
		Uint8 * pixel = (Uint8*)surface->pixels;
		int bufferPos = y * SCREEN_WIDTH + x;
		pixel += bufferPos * sizeof(Uint32);
		*(Uint32*)pixel = color;
	}
}

void SDLRenderer::setupSDLWindow(){

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
			SDL_SetColorKey(screenSurface, SDL_TRUE, SDL_MapRGB(screenSurface->format, 0, 255, 0));
			//Fill the surface white
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );

			for(int i = 0; i < 10; i++){
				for (int j = 0; j < 10; ++j) {
					PutPixel32_nolock(screenSurface, 10+i, 10+j, (Uint32)123);
				}
			}

			Main_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
			//Update the surface
			SDL_UpdateWindowSurface( window );

			//Wait two seconds
			SDL_Delay( 200 );
		}
	}

}

void SDLRenderer::killSDL(){

    //Destroy window
    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    SDL_Quit();

}

int SDLRenderer::renderImgAtPos(int x, int y, int imgX, int imgY, int width, int height, double angle){
	SDL_Rect SrcR;
	SDL_Rect DestR;
	SDL_Point originOfRot;
	SrcR.x = imgX;
	SrcR.y = imgY;
	SrcR.w = width;
	SrcR.h = height;
	DestR.x = x;
	DestR.y = y;
	DestR.w = width;
	DestR.h = height;
	int ret = SDL_RenderCopyEx(Main_Renderer,
	                   Tex,
	                   NULL,
					   &DestR,
					   angle,
					   NULL,
					   SDL_FLIP_NONE);
	return ret;
}


void SDLRenderer::SetTexture(std::string path){
	//The final texture

	    //Load image at specified path
	    SDL_Surface* picture = SDL_LoadBMP( path.c_str() );
	    if( picture == NULL )
	    {
	        fprintf( stderr, "Unable to load image %s! \n", path.c_str() );
	    }
	    else
	    {
	    	SDL_SetColorKey(picture, SDL_TRUE, SDL_MapRGB(screenSurface->format, 0, 255, 0));
	        //Create texture from surface pixels
	        Tex = SDL_CreateTextureFromSurface( Main_Renderer, picture );
	        if( Tex == NULL )
	        {
	            fprintf( stderr, "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	        }

	        //Get rid of old loaded surface
	        SDL_FreeSurface( picture );
	    }

}
