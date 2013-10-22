#include "Collisions.h"

bool isBoxBoxColliding(float box1CenterX, float box1CenterY, float box1CenterZ,
						float box1Width, float box1Height, float box1Depth,
						float box2CenterX, float box2CenterY, float box2CenterZ,
						float box2Width, float box2Height, float box2Depth)
{
	if
	(	
		(	std::abs(box1CenterX - box2CenterX)	>	(box1Width + box2Width)		/	2	)
	&&
		(	std::abs(box1CenterY - box2CenterY)	>	(box1Height + box2Height)	/	2	)
	&&
		(	std::abs(box1CenterZ - box2CenterZ)	>	(box1Depth + box2Depth)		/	2	)
	)
		return true;

	else
		return false;
}

bool isCircleCircleColliding(float circle1CenterX, float circle1CenterY, float circle1CenterZ, float circle1Radius,
							float circle2CenterX, float circle2CenterY, float circle2CenterZ, float circle2Radius)
{
	float distance = std::sqrt(std::pow(circle1CenterZ - circle1CenterY - circle1CenterX,2.0f) + std::pow(circle2CenterZ - circle2CenterY - circle2CenterX,2.0f));

	if(distance <= circle1Radius + circle1Radius)
		return true;
	
	return false;
}