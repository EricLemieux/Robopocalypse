#include "sinTable.h"

//Singleton* Singleton::pSingleton = NULL;

void sinTable::fillTable(void)
{
	for(unsigned int i = 0; i < tableSize; i++)
	{
		angle[i] = i; //TODO change i to actual desired angle parameters
		sinResult[i] = sin(float(angle[i]) * PI/180);
		cosResult[i] = cos(float(angle[i]) * PI/180);
		arcSinResult[i] = asin(float(angle[i]) * PI/180);
		arcCosResult[i] = acos(float(angle[i]) * PI/180);
	}
}


float sinTable::searchTableSin(float degree)
{
	normAngle(degree);

	for(unsigned int i = 0; i < tableSize; i++)
	{
		if((angle[i] <= degree)&&(angle[i+1] > degree))
			return sinResult[i];
	}
	return 1;
}

float sinTable::searchTableCos(float degree)
{
	normAngle(degree);

	for(unsigned int i = 0; i < tableSize; i++)
	{
		if((angle[i] <= degree)&&(angle[i+1] > degree))
			return cosResult[i];
	}
	return 1;
}

float sinTable::searchTableArcSin(float degree)
{
	normAngle(degree);

	for(unsigned int i = 0; i < tableSize; i++)
	{
		if((angle[i] <= degree)&&(angle[i+1] > degree))
			return arcSinResult[i];
	}
	return 1;
}

float sinTable::searchTableArcCos(float degree)
{
	normAngle(degree);

	for(unsigned int i = 0; i < tableSize; i++)
	{
		if((angle[i] <= degree)&&(angle[i+1] > degree))
			return arcCosResult[i];
	}
	return 1;
}