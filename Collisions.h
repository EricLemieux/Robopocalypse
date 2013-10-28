#ifndef COLLISIONS_H
#define COLLISIONS_H

#include "Engine.h"

bool isBoxBoxColliding(float box1CenterX, float box1CenterY, float box1CenterZ,
						float box1Width, float box1Height, float box1Depth,
						float box2CenterX, float box2CenterY, float box2CenterZ,
						float box2Width, float box2Height, float box2Depth);

bool isCircleCircleColliding(float circle1CenterX, float circle1CenterY, float circle1CenterZ, float circle1Radius,
                            float circle2CenterX, float circle2CenterY, float circle2CenterZ, float circle2Radius);

class collisionObjects
{
public:
	collisionObjects(){}
	~collisionObjects(){}

	//Getters
	sf::Vector3f getPos(void){return position;}
	sf::Vector3f getSize(void){return size;}

	//Setters
	void setPos(float x, float y, float z){position = sf::Vector3f(x,y,z);}
	void setPos(sf::Vector3f newPos){this->position = newPos;}
	void setSize(float width, float height, float depth){size = sf::Vector3f(width, height, depth);}
	void setSize(sf::Vector3f newSize){this->size = newSize;}

private:
	sf::Vector3f position;	//X,Y,Z of the center point
	sf::Vector3f size;		//Width, Height, Depth
};

#endif