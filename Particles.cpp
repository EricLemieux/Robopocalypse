////////////////////
//Particles.cpp
//
//Eric Lemieux, 2013
////////////////////

#include "Particles.h"

//Default constructor for the particle emitter
//Sets default ranges that a particle could use.
ParticleEmitter::ParticleEmitter(void)
{
	numOfParticles = 0;

	this->pLifeRange = Range(0,50);

	this->pSizeRange[0] = Range(1,5);
	this->pSizeRange[1] = Range(1,5);
	this->pSizeRange[2] = Range(1,5);

	this->pPositionRange[0] = Range(0,1);
	this->pPositionRange[1] = Range(-400,-401);
	this->pPositionRange[2] = Range(-400,-401);

	this->pVelocityRange[0] = Range(0,1);
	this->pVelocityRange[1] = Range(0,1);
	this->pVelocityRange[2] = Range(0,1);

	this->pAccelerationRange[0] = Range(0,1);
	this->pAccelerationRange[1] = Range(0,1);
	this->pAccelerationRange[2] = Range(0,1);

	this->pColourRange[0] = Range(0,256);
	this->pColourRange[1] = Range(0,256);
	this->pColourRange[2] = Range(0,256);
}

//Default destructor
ParticleEmitter::~ParticleEmitter(void)
{
	//TODO
}

//Sets the number of particles the emitter will create at once
void ParticleEmitter::setNumOfParticles(unsigned int num)
{
	numOfParticles = num;
}

//Initilises the particle emitter by creating all of the particles using the ranges set earlier.
void ParticleEmitter::initialise(void)
{
	for(unsigned int i = 0; i < numOfParticles; ++i)
	{
		Particle newParticle;
		newParticle.setParent(this);
		newParticle.init();
		particleList.push_back(newParticle);
	}
}

//Updates the particles if they are still alive
void ParticleEmitter::update(float dt)
{
	for(unsigned int i = 0; i < particleList.size(); ++i)
	{
		if(particleList[i].life > 0.0f)
		{
			particleList[i].update(dt);
		}
		else 
		{
			particleList.erase(particleList.begin() + i);
		}
	}
}

//Draws the particles in one big batch.
//TODO
//	-Add textures
void ParticleEmitter::draw(void)
{
	glBegin(GL_QUADS);
		for(unsigned int i = 0; i < particleList.size(); ++i)
		{
			if(particleList[i].life > 0.0f)
			{
				glColor3f(particleList[i].colour.x/256, particleList[i].colour.y/256, particleList[i].colour.z/256);
				glVertex3f(particleList[i].position.x - particleList[i].size.x/2, particleList[i].position.y + particleList[i].size.y/2, particleList[i].position.z);
				glVertex3f(particleList[i].position.x + particleList[i].size.x/2, particleList[i].position.y + particleList[i].size.y/2, particleList[i].position.z);
				glVertex3f(particleList[i].position.x + particleList[i].size.x/2, particleList[i].position.y - particleList[i].size.y/2, particleList[i].position.z);
				glVertex3f(particleList[i].position.x - particleList[i].size.x/2, particleList[i].position.y - particleList[i].size.y/2, particleList[i].position.z);
			}
		}
	glEnd();
}