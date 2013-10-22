#pragma once

#include <cmath>

bool isBoxBoxColliding(float box1CenterX, float box1CenterY, float box1CenterZ,
						float box1Width, float box1Height, float box1Depth,
						float box2CenterX, float box2CenterY, float box2CenterZ,
						float box2Width, float box2Height, float box2Depth);

bool isCircleCircleColliding(float circle1CenterX, float circle1CenterY, float circle1CenterZ, float circle1Radius,
							float circle2CenterX, float circle2CenterY, float circle2CenterZ, float circle2Radius);