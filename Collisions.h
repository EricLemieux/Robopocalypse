#ifndef COLLISIONS_H
#define COLLISIONS_H

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
	glm::vec3 getPos(void){return position;}
	glm::vec3 getSize(void){return size;}

	//Setters
	void setPos(float x, float y, float z){position = glm::vec3(x,y,z);}
	void setPos(glm::vec3 newPos){this->position = newPos;}
	void setSize(float width, float height, float depth){size = glm::vec3(width, height, depth);}
	void setSize(glm::vec3 newSize){this->size = newSize;}

private:
	glm::vec3 position;	//X,Y,Z of the center point
	glm::vec3 size;		//Width, Height, Depth
};

#endif