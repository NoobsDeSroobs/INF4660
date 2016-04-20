/*
 * TrailingPoint.cpp
 *
 *  Created on: Apr 20, 2016
 *      Author: noobsdesroobs
 */
#include "TrailingPoint.h"


TrailingPoint::TrailingPoint(float seedX, float seedY, int tailLength, float stepSize, SDLRenderer &rend){
	tail = new Cords[tailLength];

	tail[0].x = seedX;
	tail[0].y = seedY;

	for (int i = 1; i < tailLength; ++i) {
		tail[i].x = NOT_SET;
		tail[i].y = NOT_SET;
	}
	this->stepSize = stepSize;
	tailSize = tailLength;
	renderer = &rend;
}

void TrailingPoint::iterate(){
	for (int i = tailSize-1; i > 0; --i) {
			if(tail[i].x == NOT_SET){

			}else{
				tail[i].x = tail[i-1].x;
				tail[i].y = tail[i-1].y;
			}
		}

	//tail[0].x = WilhelmMath.getNewX();
	//tail[0].y = WilhelmMath.getNewY();

}

void TrailingPoint::render(){
	for (int i = 0; i < tailSize; ++i) {
		if(tail[i].x == NOT_SET){
			//We are done.
			return;
		}
		//Render the pixel tail[i] with with an alpha of (tailSize-i)/tailSize.
		int colour = ((tailSize-i)/(double)tailSize)*255;
		renderer->PutPixel32_nolock(renderer->getMainSurface(),
				(int)tail[i].x, (int)tail[i].y,
				renderer->RGBA2INT(colour, colour, colour, colour));
	}
}




void Seeder(){
	//Compute an initial streamline and make it current.
	int currentStreamline = 0;
	//Queue streamline Queue
	bool finished = false;
	bool validCandidate = false;
	bool empty = false;
	while(!finished){
		while(!empty && !validCandidate){
			//Select a seedpoint with minimum d distance to current streamline
		}

		if(validCandidate){
			//Compute streamline and add to streamline queue.
		}else{
			if(true/*Queue is empty*/){
				finished = true;
			}else{
				currentStreamline = 1;//queue.next()
			}
		}
	}

}
