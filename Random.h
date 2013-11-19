////////////////////
//Random.h
//
//Eric Lemieux, 2013
////////////////////

#pragma once

//////////
//Includes
//////////
#include <iostream>
#include <time.h>

//Generate a random seed using time for a more actual random
inline void genRandSeed()
{
	srand(time(NULL));
}

//Generate a random value between the minimum and maximum value
inline float random(const float min, const float max)
{	
	return float(rand() % int(max - min) + min);
}