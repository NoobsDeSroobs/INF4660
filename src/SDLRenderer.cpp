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

void SDLRenderer::setupSDLWindow(string windName, bool greyScale){

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}else{
		//Create window
		window = SDL_CreateWindow( windName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		} else {
			//Get window surface
			screenSurface = SDL_GetWindowSurface( window );
			if(greyScale){
				SDL_Color colors[256];
				int i;

				for(i = 0; i < 256; i++)
				{
				    colors[i].r = colors[i].g = colors[i].b = i;
				}
			}else{
				SDL_SetColorKey(screenSurface, SDL_TRUE, SDL_MapRGB(screenSurface->format, 0, 255, 0));
			}
			//Fill the surface white
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );


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

	SDL_Rect DestR;
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

void SDLRenderer::drawLine(std::vector<WMpoint> &curve){
	SDL_Point points[curve.size()];
	for (uint i = 0; i < curve.size(); ++i) {
		points[i].x = (int)curve[i].x;
		points[i].y = (int)curve[i].y;
	}
	for (uint i = 0; i < curve.size()-1; ++i) {
		if(points[i].x > 0 && points[i].y > 0 && points[i].x < SCREEN_WIDTH && points[i].y < SCREEN_HEIGHT && points[i+1].x > 0 && points[i+1].y > 0 && points[i+1].x < SCREEN_WIDTH && points[i+1].y < SCREEN_HEIGHT){
			//fprintf(stderr, "DrawLineasdfasdf %d, %d\n", points[i].x, points[i].y);
			SDL_RenderDrawLine(Main_Renderer, points[i].x, points[i].y, points[i+1].x, points[i+1].y );
		}
	}

}
