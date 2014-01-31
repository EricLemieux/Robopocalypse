#include "HUD.h"

Bar::Bar(void)
{
	maxValue = currentValue = 1.0f;
	minValue				= 0.0f;

	scaleDirection = scaleRight;

	startPosOfBar	= -0.98f;
	endPosOfBar		= -0.2f;

	barWidth = 0.3f;

	timeValue = 0.0f;
	currentValue = 1.0f;
	previousValue = 1.0f;
}
Bar::Bar(float maxVal, short direction, float startPos, float width, glm::vec3 colour, float heightTranslation)
{
	if(maxVal <= 1.0f)
		maxValue = currentValue = maxVal;
	else
		maxValue = currentValue = 1.0f;
	minValue				= 0.0f;

	scaleDirection = direction;

	startPosOfBar = startPos;

	barWidth = width;

	barColour = colour;

	barTranslation.y = heightTranslation;

	timeValue = 0.0f;
	previousValue = 1.0f;
}
Bar::~Bar(void)
{
}

void Bar::setCurrentValue(float newValue)
{
	currentValue = newValue;
}

void Bar::setPreviousValue(float newValue)
{
	if(newValue < currentValue)
	{
		previousValue = newValue;
	}
}

void Bar::setTimeValue(float newValue)
{
	if(newValue > 0.0f && newValue < 1.0f)
	{
		timeValue = newValue;
	}
}

void Bar::update(void)
{
	if(currentValue <= minValue)
	{
		currentValue = minValue;
	}

	currentValue *= maxValue;
	
	endPosOfBar = startPosOfBar + (scaleDirection * currentValue);
}
void Bar::update(float t, Bar &otherBar)
{
	if(this->currentValue >= otherBar.getCurrentValue()+0.002f || this->currentValue <= otherBar.getCurrentValue()-0.002f)
	{
		if(timeValue < 1.0f)
		{
			timeValue += t;
			this->currentValue = LERP( this->previousValue, otherBar.getCurrentValue(), timeValue);
		}
		else
		{
			timeValue = 0.0f;
			this->currentValue = otherBar.getCurrentValue();
		}
	}
	else
	{
		this->currentValue	= otherBar.getCurrentValue();
		this->previousValue = otherBar.getCurrentValue();
		timeValue = 0.0f;
	}

	endPosOfBar = startPosOfBar + (scaleDirection * currentValue);
}

void Bar::draw(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//Set Colour
	glColor3f(barColour.r, barColour.g, barColour.b);

	glTranslatef(0.0f, barTranslation.y, 0.0f);

	glBegin(GL_TRIANGLES);
		//Top right triangle
		glVertex3f( startPosOfBar,	+barWidth, 0.0f);
		glVertex3f( endPosOfBar,	+barWidth, 0.0f);
		glVertex3f( endPosOfBar,	-barWidth, 0.0f);
		//Bottom left triangle		
		glVertex3f( startPosOfBar,	+barWidth, 0.0f);
		glVertex3f( startPosOfBar,	-barWidth, 0.0f);
		glVertex3f( endPosOfBar,	-barWidth, 0.0f);
	glEnd();

	//Reset the colour to white
	glColor3f(1.0f, 1.0f, 1.0f);
}


HUD::HUD(void)
{
	//init the different bars
	p1Health		= Bar(0.7f, scaleLeft, -0.8f, 0.3f, glm::vec3(1.0f, 0.0f, 0.0f), 0.5f);
	p1HealthBack	= Bar(0.7f, scaleLeft, -0.8f, 0.3f, glm::vec3(0.7f, 0.2f, 0.2f), 0.5f);
	p1Shield		= Bar(0.7f, scaleLeft, -0.8f, 0.3f, glm::vec3(0.2f, 0.5f, 0.8f), -0.5f);
	p1ShieldBack	= Bar(0.7f, scaleLeft, -0.8f, 0.3f, glm::vec3(0.7f, 0.2f, 0.2f), -0.5f);

	p2Health		= Bar(0.7f, scaleRight, 0.8f, 0.3f, glm::vec3(1.0f, 0.0f, 0.0f), 0.5f);
	p2HealthBack	= Bar(0.7f, scaleRight, 0.8f, 0.3f, glm::vec3(0.7f, 0.2f, 0.2f), 0.5f);
	p2Shield		= Bar(0.7f, scaleRight, 0.8f, 0.3f, glm::vec3(0.2f, 0.5f, 0.8f), -0.5f);
	p2ShieldBack	= Bar(0.7f, scaleRight, 0.8f, 0.3f, glm::vec3(0.7f, 0.2f, 0.2f), -0.5f);

	//init the images for the back of the HUD
}
HUD::~HUD(void)
{
}

void HUD::update(Player *player1, Player *player2, float t)
{
	//Set with current values
	p1Health.setCurrentValue(player1->getHealth()/100.0f);
	p1Shield.setCurrentValue(player1->getShield()/50.0f	);
	p2Health.setCurrentValue(player2->getHealth()/100.0f);
	p2Shield.setCurrentValue(player2->getShield()/50.0f	);
	
	//p1HealthBack.setCurrentValue(LERP(p1Health.getPreviousValue(),p1Health.getCurrentValue(),t));
	p1HealthBack.setPreviousValue(player1->getHealth()/100.0f);
	p1ShieldBack.setPreviousValue(player1->getShield()/50.0f);
	p2HealthBack.setPreviousValue(player2->getHealth()/100.0f);
	p2ShieldBack.setPreviousValue(player2->getShield()/50.0f);

	//Update all of the bars now
	p1Health.update();
	p1HealthBack.update(t, p1Health);
	p1Shield.update();
	p1ShieldBack.update(t, p1Shield);
	p2Health.update();
	p2HealthBack.update(t, p2Health);
	p2Shield.update();
	p2ShieldBack.update(t, p2Shield);
}
void HUD::draw(void)
{
	glViewport(0,WINDOW_HEIGHT-HUD_HEIGHT,WINDOW_WIDTH,HUD_HEIGHT);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10,10,-10,10,1,1000);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glDisable(GL_CULL_FACE);

	glDisable(GL_TEXTURE_2D);

	p1Health.draw();
	p1HealthBack.draw();
	p1Shield.draw();
	p1ShieldBack.draw();
	p2Health.draw();
	p2HealthBack.draw();
	p2Shield.draw();
	p2ShieldBack.draw();

	glPopMatrix();
}