//HUD system
//Eric Lemieux, 2014.

#pragma once

//Includes
//#include "AnimationMath.h"
#include "Engine.h"

enum 
{
	scaleRight = -1,
	scaleNULL,
	scaleLeft
};

//Used for health and shield bars
class Bar
{
private:
	short scaleDirection;
	float maxValue, minValue, currentValue;

	float startPosOfBar, endPosOfBar;

	float barWidth;

	glm::vec3 barColour;
	glm::vec3 barTranslation;

	float previousValue;

	float timeValue;

public:
	Bar(void);
	Bar(float maxVal, short direction, float startPos, float width, glm::vec3 colour, float heightTranslation);
	~Bar(void);

	void update(void);
	void update(float t, Bar &otherBar);
	void draw(void);

	void setCurrentValue(float newValue);
	inline float getCurrentValue(void){ return currentValue; }

	void setPreviousValue(float newValue);
	inline float getPreviousValue(void){ return previousValue; }

	void setTimeValue(float newValue);
	inline float getTimeValue(void){ return timeValue; }
};

class HUD
{
public:
	HUD(void);
	~HUD(void);

	void update(Player *player1, Player *player2, float t);
	void draw(void);

	Bar p1Health, p1HealthBack, p1Shield, p1ShieldBack,
		p2Health, p2HealthBack, p2Shield, p2ShieldBack;
};