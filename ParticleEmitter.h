#pragma once

#include "Particle.h"

class ParticleEmitter : public GameObject{
private:
	glm::vec3 pos;
	glm::vec3 vel;
	Node *emitterNode;
	VertexBuffer *VBO;

	std::vector<Particle*> *particleList;

	bool isActive;
	int activeCounter;
public:
	ParticleEmitter();
	~ParticleEmitter();

	void update(ParticleType type);

	glm::vec3 getPos(){return pos;}
	void setPos(glm::vec3 newPos){pos = newPos;}

	void ActivateEmitter(){isActive = true;}
	void DeactivateEmitter();

	std::vector<Particle*>* getParticleList(){return particleList;}

};