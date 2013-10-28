#include "Animation.h"


Animation::Animation(std::string spriteSheetFilename, float width, float height)
{
	spacingX = width;
	spacingY = height;
	this->loadSpriteSheet(spriteSheetFilename.c_str());
	layerID = 0;
}


Animation::~Animation(void)
{
}

void Animation::loadSpriteSheet(const char *filename)
{
	spriteTexture.loadFromFile(filename);
	sprite.setTexture(spriteTexture);
	sprite.setTextureRect(sf::IntRect(0,0,spacingX,spacingY));
}

void Animation::setNumberOfAnim(float num)
{
	numberOfAnimations = num;
	animations.reserve(num);
	for(int i=0;i<num;i++)
	{
		SpriteAnimation *anim = new SpriteAnimation();
		animations.push_back(anim);
	}
}

void Animation::addAnimFrame(int animationNumber, float x, float y)
{
	sf::IntRect temp(x,y,spacingX,spacingY);
	animations[animationNumber]->coords.push_back(temp);
}

void Animation::addAnimRow(int animationNumber, float startX, float startY, int numFrames)
{
	float x,y;
	x = startX;
	y = startY;
	for(int i=0;i<numFrames;i++)
	{
		addAnimFrame(animationNumber, x,y);
		x += spacingX;
		//y += spacingY;
	}
}

void Animation::setCurrentAnim(float anim)
{
	currentAnimation = anim;
		if(currentAnimation >= animations.size()) 
			currentAnimation = 0;
}

void Animation::nextFrame()
{
	animations[currentAnimation]->nextFrame();
	int cFrame = animations[currentAnimation]->currentFrame;
	sprite.setTextureRect(animations[currentAnimation]->coords[cFrame]);
}