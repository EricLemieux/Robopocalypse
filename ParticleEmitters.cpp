#include "ParticleEmitters.h"

ParticleEmitter::ParticleEmitter(){
	emitterNode = GetNode();
	
	vel = glm::vec3(0,0,0);
	pos = emitterNode->GetLocalPosition();

	isActive = false;
	timer = 0;
}

ParticleEmitter::~ParticleEmitter(){
	free(emitterNode);
}

void ParticleEmitter::update(){
	pos = emitterNode->GetLocalPosition();
	emitterNode->SetLocalPosition(pos);
}

void ParticleEmitter::Activate(){
	isActive = true;
}

void ParticleEmitter::Deactivate(){
	isActive = false;
}

SmokeEmitter::SmokeEmitter(){
	emitterNode = GetNode();
	
	vel = glm::vec3(0,0,0);
	pos = emitterNode->GetLocalPosition();

	isActive = false;
	timer = 0;

}

SmokeEmitter::~SmokeEmitter(){}

void SmokeEmitter::update(float playerVel){
	pos = emitterNode->GetLocalPosition();
	if(isActive){
		++timer;
		if(timer%10 == 0){
			SmokeParticle temp(playerVel),temp2(playerVel+2.f)
				,temp3(playerVel-2.f);
			particleList.push_back(temp);
			particleList.push_back(temp2);
			particleList.push_back(temp3);
			particleList.push_back(temp);
		}
		for(int i = 0, size = particleList.size(); i < size; ++i){
			particleList[i].update();
		}
	} else {
		particleList.clear();
		timer = 0;
	}
	emitterNode->SetLocalPosition(pos);
}

FireEmitter::FireEmitter(){
	emitterNode = GetNode();
	
	vel = glm::vec3(0,0,0);
	pos = emitterNode->GetLocalPosition();

	isActive = false;
	timer = 0;
}

FireEmitter::~FireEmitter(){}

void FireEmitter::update(){
	pos = emitterNode->GetLocalPosition();
	if(isActive){
		++timer;
		if(timer%5 == 0){
			FireParticle temp;
			particleList.push_back(temp);
			particleList.push_back(temp);
			particleList.push_back(temp);
			particleList.push_back(temp);
		}
		for(int i = 0, size = particleList.size(); i < size; ++i){
			particleList[i].update();
		}
	} else {
		particleList.clear();
		timer = 0;
	}
	emitterNode->SetLocalPosition(pos);
}

SparkEmitter::SparkEmitter(){
	emitterNode = GetNode();
	
	vel = glm::vec3(0,0,0);
	pos = emitterNode->GetLocalPosition();

	isActive = false;
	timer = 0;

	nextSpark = 0;
}

SparkEmitter::~SparkEmitter(){}

void SparkEmitter::update(){
	pos = emitterNode->GetLocalPosition();
	if(isActive){
		++timer;
		if(timer%nextSpark == 0){
			SparkParticle temp;
			particleList.push_back(temp);
			nextSpark = rand()%60+1;
		}
		for(int i = 0, size = particleList.size(); i < size; ++i){
			particleList[i].update();
		}
	} else {
		particleList.clear();
		timer = 0;
		nextSpark = 0;
	}
	emitterNode->SetLocalPosition(pos);
}

ImpactEmitter::ImpactEmitter(){
	emitterNode = GetNode();
	
	vel = glm::vec3(0,0,0);
	pos = emitterNode->GetLocalPosition();

	isActive = false;
	timer = 0;
}

ImpactEmitter::~ImpactEmitter(){}

void ImpactEmitter::update(){
	pos = emitterNode->GetLocalPosition();
	if(isActive){
		++timer;
		if(timer%10 == 0){
			ImpactParticle temp;
			particleList.push_back(temp);
			
		}
		for(int i = 0, size = particleList.size(); i < size; ++i){
			particleList[i].update();
		}
	} else {
		particleList.clear();
		timer = 0;
		
	}
	emitterNode->SetLocalPosition(pos);
}