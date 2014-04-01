#pragma once

#include "Particle.h"



class ParticleEmitter : public GameObject{
private:
	glm::vec3 pos;
	glm::vec3 vel;
	Node *emitterNode;
	VertexBuffer *VBO;

	//int particleListSize;

	//std::vector<Particle*> particleList;
	//Particle* particleList[50];

	bool isActive;
	int activeCounter;

	int particleNum;
public:
	Particle* particleList[50];
	ParticleEmitter();
	~ParticleEmitter();

	void update(ParticleType type, Player *player1, Player *player2);

	glm::vec3 getPos(){return pos;}
	void setPos(glm::vec3 newPos){pos = newPos;}

	void ActivateEmitter(){isActive = true;}
	void DeactivateEmitter();

	Particle* getParticleList(){return particleList[0];}
	
	int getParticleListSize(){return 50;}

	void incrementNum(int &particleNum);

};