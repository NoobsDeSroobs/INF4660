/*
 * TrailingPoint.h
 *
 *  Created on: Apr 20, 2016
 *      Author: noobsdesroobs
 */

#ifndef TRAILINGPOINT_H_
#define TRAILINGPOINT_H_
#include "SDLRenderer.h"

class TrailingPoint{

	struct Cords{
		float x;
		float y;
	} typedef Cords;

	enum TAIL_STATUS{
		NOT_SET = -1
	};

	Cords* tail;
	float stepSize;
	int tailSize;
	SDLRenderer *renderer;

	TrailingPoint(float seedX, float seedY, int tailLength, float stepSize, SDLRenderer &rend);

	void render();

	void iterate();



};



#endif /* TRAILINGPOINT_H_ */
