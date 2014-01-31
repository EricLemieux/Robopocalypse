#pragma once

#include <glm\glm.hpp>	
#include "glm\glm\ext.hpp"

template <class T> 
class TSingleton
{
public:
	static T * GetInstance()
	{
		static T obj;
		return &obj;
	}

private:
	TSingleton(void){}
	~TSingleton(void){}

	//static Singleton* pSingleton;
};

#define SINGLETON(T) protected:friend class TSingleton< T >;T(){};  
#define SINGLETONGETINSTANCE(T) TSingleton< T >::GetInstance();



#define PI 3.14159265359

const static unsigned int tableSize = 360;

class sinTable
{
public:
	SINGLETON(sinTable);
	//~sinTable(){};
public:
	//Fill sineTable, run once at start up
	void fillTable();
	//search table for values
	float searchTableSin(float degree); //sin
	float searchTableCos(float degree); //cos
	float searchTableArcSin(float degree);//arcsin
	float searchTableArcCos(float degree);//arccos

	float normAngle(float degree)
	{
		if(degree < 0.0f)
			return normAngle(degree + 360.0f);
		else if(degree >= 360.0f)
			return normAngle(degree - 360.0f);
		return(degree);
	}

private:
	float angle[tableSize];
	//Sin
	float sinResult[tableSize];
	float arcSinResult[tableSize];
	//Cos
	float cosResult[tableSize];
	float arcCosResult[tableSize];


};