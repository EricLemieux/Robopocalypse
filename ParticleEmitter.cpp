#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(){
	//emitterNode = this->GetNode();
	isActive = false;
	pos = glm::vec3(0,0,0);
	vel = glm::vec3(0,0,0);

	VBO = new VertexBuffer;
	VBO->Initialize(1,false,false,false);
	//this->AttachModel(OBJModel("Resources/Models/Ball.obj").GetVBO());
	this->AttachModel(VBO);

	particleList = new std::vector<Particle*>;

	activeCounter = 0;
}
ParticleEmitter::~ParticleEmitter(){

}

void ParticleEmitter::update(ParticleType type){
	pos = this->GetNode()->GetLocalPosition();
	if(isActive){
		if(type == BASIC){
			particleList->push_back(new Particle(type));
			this->GetNode()->AttachNode(particleList->back()->GetNode());
		} else if (type == SMOKE && activeCounter%7 == 0){
			particleList->push_back(new Particle(type));
			this->GetNode()->AttachNode(particleList->back()->GetNode());	
			particleList->push_back(new Particle(type));
			this->GetNode()->AttachNode(particleList->back()->GetNode());	
			particleList->push_back(new Particle(type));
			this->GetNode()->AttachNode(particleList->back()->GetNode());	
			particleList->push_back(new Particle(type));
			this->GetNode()->AttachNode(particleList->back()->GetNode());			
		} else if (type == SPARK && activeCounter%17 == 0){
			particleList->push_back(new Particle(type));
			this->GetNode()->AttachNode(particleList->back()->GetNode());
		} else if (type == SHIELD){
			particleList->push_back(new Particle(type));
			this->GetNode()->AttachNode(particleList->back()->GetNode());
		}
		
		for(int i = 0; i<particleList->size();++i){
			if(particleList->at(i)->update(type,this->GetNode()) == false){
				this->GetNode()->DetatchNode(particleList->at(i)->getParticleNode());
				particleList->at(i)->release();
				particleList->erase(particleList->begin()+i);
			}
		}
		++activeCounter;
	}
	int a = 0;
	this->GetNode()->SetLocalPosition(pos);
}

void ParticleEmitter::DeactivateEmitter(){
	isActive = false;
	for(int i = 0, size = particleList->size(); i < size; ++i){
		this->GetNode()->DetatchNode(particleList->at(i)->getParticleNode());
		particleList->at(i)->release();
	}
	particleList->clear();
}