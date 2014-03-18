#include "Particles.h"

Particle::Particle(){
	pos = glm::vec3(0,0,0);
	vel = glm::vec3(0,0,0);

	particleNode = GetNode();

	life = 0;
	currentLife = 0;
}
Particle::~Particle(){
	free(particleNode);
}

void Particle::update(){}

bool Particle::isAlive(){
	if(currentLife > life)
		return false;
	return true;
}

SmokeParticle::SmokeParticle(float playerVel){
	
	vel = glm::vec3(0,0,0);

	particleNode = GetNode();

	pos = particleNode->GetLocalPosition();
	pos.x += rand()%4 - 2;
	particleNode->SetLocalPosition(pos);

	currentLife = 0;
	life = rand()%40 + 60;

	initXVel = playerVel * 0.25f;
}

SmokeParticle::~SmokeParticle(){}

void SmokeParticle::update(){
	pos = particleNode->GetLocalPosition();

	vel.y = 3.f;
	vel.x += initXVel;
	pos.x += vel.x;
	pos.y += vel.y;

	++currentLife;

	particleNode->SetLocalPosition(pos);
}

FireParticle::FireParticle(){
	
	vel = glm::vec3(0,0,0);

	particleNode = GetNode();

	pos = particleNode->GetLocalPosition();
	pos.x += rand()%4 - 2;
	particleNode->SetLocalPosition(pos);

	currentLife = 0;
	life = rand()%60 + 40;
}

FireParticle::~FireParticle(){}

void FireParticle::update(){
	pos = particleNode->GetLocalPosition();
	vel.y = 1.f;
	pos.y += vel.y;

	++currentLife;

	particleNode->SetLocalPosition(pos);
}

SparkParticle::SparkParticle(){
	vel = glm::vec3(0,0,0);

	particleNode = GetNode();

	glm::vec3 temp(rand()&10-5,rand()&10-5,rand()&4-2);

	pos = particleNode->GetLocalPosition();
	pos += temp;
	particleNode->SetLocalPosition(pos);

	currentLife = 0;
	life = rand()%10+1;
}

SparkParticle::~SparkParticle(){}

void SparkParticle::update(){
	pos = particleNode->GetLocalPosition();
	++currentLife;
	particleNode->SetLocalPosition(pos);
}

ImpactParticle::ImpactParticle(){
	vel = glm::vec3(0,0,0);


	particleNode = GetNode();

	pos = particleNode->GetLocalPosition();

	currentLife = 0;
	life = 25;
}

ImpactParticle::~ImpactParticle(){}

void ImpactParticle::update(){
	pos = particleNode->GetLocalPosition();
	particleNode->SetScale(glm::vec3(currentLife,currentLife,1));
	particleNode->SetLocalPosition(pos);
}

