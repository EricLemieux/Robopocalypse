
#pragma once

//////////
//Includes
//////////
#include <vector>
#include <glm\glm.hpp>
//#include <SFML\OpenGL.hpp>
#include "Random.h"
//#include <SFML\Graphics.hpp>
#include "GLFW\glfw3.h"
struct Range;
struct Particle;
class ParticleEmitter;
class ParticleManager;

//////////
//Range
//
//A minimum and maximum value for what the randoms can generate
//////////
struct Range
{
	//The limits for the range
	float min;
	float max;

	//Constructors
	inline Range::Range(void){	}
	inline Range::Range(float minVal, float maxVal)
	{
		min = minVal;
		max = maxVal;
	}
};

//////////
//ParticleEmitter
//
//Emits a set number of particles in a direction based on the simple physics the user sets
//////////
class ParticleEmitter
{
private:
	unsigned int numOfParticles;

public:
	//TODO GETTERS
	Range pLifeRange;
	Range pSizeRange[3];
	Range pPositionRange[3];
	Range pVelocityRange[3];	
	Range pAccelerationRange[3];
	Range pColourRange[3];

	//parent 
	ParticleManager *parent;

	//What texture
	unsigned int particleTexture;

	//Default constructor and destructor
	ParticleEmitter(void);
	~ParticleEmitter(void);

	//Only to be used before initiallising, or else will have no effect;
	void setNumOfParticles(unsigned int num);

	//Creates all of the particles
	void initialise(void);

	//Stores all of the particles in a single place
	//TODO
	//	-Use something more efficent
	std::vector<Particle> particleList;
	
	//Updates all of the particles if alive
	void update(float dt);

	//Draws all the particles after there position has been updated
	void draw(void);

	//Set the limits for the various aspects with user input
	inline void setLifeRange(Range lr){	pLifeRange = lr;	}
	inline void setSizeRange(Range sr[3]){	pSizeRange[0] = sr[0]; pSizeRange[1] = sr[1]; pSizeRange[2] = sr[2];	}
	inline void setPosRange(Range pr[3]){	pPositionRange[0] = pr[0]; pPositionRange[1] = pr[1]; pPositionRange[2] = pr[2];	}
	inline void setVelRange(Range vr[3]){	pVelocityRange[0] = vr[0]; pVelocityRange[1] = vr[1]; pVelocityRange[2] = vr[2];	}
	inline void setAcelRange(Range ar[3]){	pAccelerationRange[0] = ar[0]; pAccelerationRange[1] = ar[1]; pAccelerationRange[2] = ar[2];	}
	inline void setColourRange(Range cr[3]){	pColourRange[0] = cr[0]; pColourRange[1] = cr[1]; pColourRange[2] = cr[2];	}
};

//////////
//ParticleManager
//
//Manages all of the emitters
//////////
class ParticleManager
{
private:
	//Vector of all emitters
	//TODO
	//	-replace vector with something more efficent
	std::vector<ParticleEmitter> particleSystems;

public:
	//Default constructor
	inline ParticleManager(void)
	{
		//init some textures for particles

		//replace?
		//Spark Texture
		//sf::Image sparkParticleImageMap;
		//if(!sparkParticleImageMap.loadFromFile("resources/spark.png"))
		//{
		//	std::cout<<"error loading spark texture for particle\n";
		//}

		//glGenTextures(1,&sparkTex);
		//glBindTexture(GL_TEXTURE_2D,sparkTex);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, sparkParticleImageMap.getSize().x, sparkParticleImageMap.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, sparkParticleImageMap.getPixelsPtr());
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//
		////Smoke Texture
		//sf::Image smokeParticleImageMap;
		//if(!smokeParticleImageMap.loadFromFile("resources/smoke.png"))
		//{
		//	std::cout<<"error loading smoke texture for particle\n";
		//}
		//
		//glGenTextures(1,&smokeTex);
		//glBindTexture(GL_TEXTURE_2D,smokeTex);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, smokeParticleImageMap.getSize().x, smokeParticleImageMap.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, smokeParticleImageMap.getPixelsPtr());
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//
		////Shield Texture
		//sf::Image shieldParticleImageMap;
		//if(!shieldParticleImageMap.loadFromFile("resources/shield.png"))
		//{
		//	std::cout<<"error loading shield texture for particle\n";
		//}
		//
		//glGenTextures(1,&shieldTex);
		//glBindTexture(GL_TEXTURE_2D,shieldTex);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, shieldParticleImageMap.getSize().x, shieldParticleImageMap.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, shieldParticleImageMap.getPixelsPtr());
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//
		////Default Texture
		//sf::Image defaultParticleImageMap;
		//if(!defaultParticleImageMap.loadFromFile("resources/default.bmp"))
		//{
		//	std::cout<<"error loading default texture for particle\n";
		//}
		//
		//glGenTextures(1,&defaultTex);
		//glBindTexture(GL_TEXTURE_2D,defaultTex);
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, defaultParticleImageMap.getSize().x, defaultParticleImageMap.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, defaultParticleImageMap.getPixelsPtr());
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	//Add an emmiter to the list of particle systems
	//inline void addEmmiter(ParticleEmitter *newEmmiter)
	inline void addEmmiter(ParticleEmitter newEmmiter)
	{
		//newEmmiter.parent = this;
		particleSystems.push_back(newEmmiter);
	}
	inline void delEmmiter(ParticleEmitter &newEmmiter)
	{
		//TODO
	}

	//Loop through the list of particle systems and update them before they are drawn to the screen.
	inline void update(float dt)
	{
		for(unsigned int i = 0; i < particleSystems.size(); ++i)
		{
			if(particleSystems[i].particleList.size() == 0)
			{
				particleSystems.erase(particleSystems.begin() + i);
			}
			else
			{
				particleSystems[i].update(dt);
			}
		}
	}
	//Loop through the list of particle systems and draw them to the screen.
	inline void draw(void)
	{
		for(unsigned int i = 0; i < particleSystems.size(); ++i)
		{
			particleSystems[i].draw();
		}
	}

	//DATA

	//Textures
	GLuint sparkTex;
	GLuint defaultTex;
	GLuint smokeTex;
	GLuint shieldTex;
};

//////////
//Particle
//
//The smallest part of the particle system
//////////
struct Particle
{
public:
	//Default constructor
	inline Particle::Particle()	{	}

	//Initialise the particle
	//Uses the ranges created in the emitter to create each particle
	inline void init(void)
	{
		life = random(parent->pLifeRange.min, parent->pLifeRange.max);
		size = glm::vec3(random(parent->pSizeRange[0].min,parent->pSizeRange[0].max), random(parent->pSizeRange[1].min,parent->pSizeRange[1].max), random(parent->pSizeRange[2].min,parent->pSizeRange[2].max));
		position = glm::vec3(random(parent->pPositionRange[0].min,parent->pPositionRange[0].max), random(parent->pPositionRange[1].min,parent->pPositionRange[1].max), random(parent->pPositionRange[2].min,parent->pPositionRange[2].max));
		velocity = glm::vec3(random(parent->pVelocityRange[0].min,parent->pVelocityRange[0].max), random(parent->pVelocityRange[1].min,parent->pVelocityRange[1].max), random(parent->pVelocityRange[2].min,parent->pVelocityRange[2].max));
		acceleration = glm::vec3(random(parent->pAccelerationRange[0].min,parent->pAccelerationRange[0].max), random(parent->pAccelerationRange[1].min,parent->pAccelerationRange[1].max), random(parent->pAccelerationRange[2].min,parent->pAccelerationRange[2].max));
		colour = glm::vec3(random(parent->pColourRange[0].min,parent->pColourRange[0].max), random(parent->pColourRange[1].min,parent->pColourRange[1].max), random(parent->pColourRange[2].min,parent->pColourRange[2].max));
	}

	//Set the parent of this particle, used to get the information about the ranges in initialisation
	inline void setParent(ParticleEmitter *newParent)
	{
		parent = newParent;
	}

	ParticleEmitter *parent;

	float life;

	glm::vec3 size;

	glm::vec3 position;

	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::vec3 colour;

	//Update the particle using simple physics
	inline void update(float dt)
	{
		life -= dt;

		position += velocity * dt + 0.5f * acceleration * dt * dt;
		velocity += acceleration * dt;
	}
};