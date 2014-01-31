#include "Engine.h"

//check if there is a box-box collision betweent the boxes given
//Used frequently by the player class
bool isBoxBoxColliding(float box1CenterX, float box1CenterY, float box1CenterZ,
                        float box1Width, float box1Height, float box1Depth,
                        float box2CenterX, float box2CenterY, float box2CenterZ,
                        float box2Width, float box2Height, float box2Depth)
{
        if
        (        
                (	std::abs(box1CenterX - box2CenterX)	<	(box1Width + box2Width)		/	2	)
        &&
                (	std::abs(box1CenterY - box2CenterY)	<	(box1Height + box2Height)	/	2	)
        &&
                (	std::abs(box1CenterZ - box2CenterZ)	<	(box1Depth + box2Depth)		/	2	)
        )
                return true;
        else
                return false;
}

//Same as above except using vectors 
bool isBoxBoxColliding(glm::vec3 box1Center, glm::vec3 box1Size, glm::vec3 box2Center, glm::vec3 box2Size)
{
	return isBoxBoxColliding(box1Center.x, box1Center.y, box1Center.z,
							box1Size.x, box1Size.y, box1Size.z,
							box2Center.x, box2Center.y, box2Center.z,
							box2Size.x, box2Size.y, box2Size.z);
}

//checks to see if two circles are within eachothers radius
bool isCircleCircleColliding(float circle1CenterX, float circle1CenterY, float circle1CenterZ, float circle1Radius,
                                                        float circle2CenterX, float circle2CenterY, float circle2CenterZ, float circle2Radius)
{
        float distance = std::sqrt(std::pow(circle1CenterZ - circle1CenterY - circle1CenterX,2.0f) + std::pow(circle2CenterZ - circle2CenterY - circle2CenterX,2.0f));

        if(distance <= circle1Radius + circle1Radius)
                return true;
        
        return false;
}