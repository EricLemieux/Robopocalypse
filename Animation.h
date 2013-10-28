#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>


class SpriteAnimation 
{
public:
	SpriteAnimation(void)
	{
		currentFrame = 0;
		doesLoop = true;
		pong = false;
		ping = false;
	}
	~SpriteAnimation(void){}
	
	void nextFrame()
	{
		int numFrames = coords.size();

		if(!pong)
		{
			currentFrame++;

			if(currentFrame >= numFrames)
			{
				if(doesLoop) currentFrame = 0;
				else currentFrame = numFrames - 1;
			}
		}else if(pong){
			nextFramePing();
		}
	}

	void nextFramePing(){ //Pingpong animation
		int numFrames = coords.size() - 1;

		if(!ping){
			currentFrame++;
		}else if(ping){
			currentFrame--;}
		
		if(currentFrame >= numFrames){
			ping = true;
		}else if(currentFrame <= 0){
			ping = false;
		}
	}

	void setLooping(bool loop) {doesLoop = loop;}

	void setPingPong(bool p){pong = p;}

	/* each animation has a list of x,y coordinates
	   that can be cycled through */
	std::vector<sf::IntRect> coords;
	bool doesLoop;
	bool pong, ping;
	int currentFrame;
};

class Animation
{
public:

	Animation(std::string spriteSheetFilename, float width, float height);
	~Animation(void);

	void loadSpriteSheet(const char *filename);
	void setNumberOfAnim(float num);
	void addAnimFrame(int animationNumber, float x, float y);
	void addAnimRow(int animationNumber, float startX, float startY, int numFrames);
	void setCurrentAnim(float anim);
	void nextFrame();

	void setPosition(float x, float y) {
		sprite.setPosition(x,y);
	}

	//DATA
	int numberOfAnimations;
	int currentAnimation;
	
	float spacingX;
	float spacingY;

	float layerID; 

	//TEXTURE
	sf::Texture spriteTexture;

	sf::Sprite sprite;
	
	/* list of animations */
	std::vector<SpriteAnimation*> animations;
};

