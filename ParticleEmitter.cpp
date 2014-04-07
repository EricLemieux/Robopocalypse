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

	//particleList = new std::vector<Particle*>;
	//particleList.resize(1000);
	//particleList.reserve(1000);

	particleNum = 0;

	for(int i = 0; i < 50; ++i){
		particleList[i] = new Particle;
		this->GetNode()->AttachNode(particleList[i]->GetNode());
	}

	activeCounter = 0;
}
ParticleEmitter::~ParticleEmitter(){
	//delete(particleList);
}

void ParticleEmitter::update(ParticleType type, Player *player1, Player *player2){
	pos = this->GetNode()->GetLocalPosition();
	if((activeCounter == 0) && (isActive)){
		for(int i = 0; i < 50; ++i){
			particleList[i]->InitTex(type);
		}
	}
	if(isActive){
		if(type == BASIC){
			particleList[particleNum]->Init(type);
			//this->GetNode()->AttachNode(particleList[particleNum]->GetNode());
			incrementNum(particleNum);
			//particleList.push_back(new Particle(type));
			
		} else if (type == SMOKE && (activeCounter%7 == 0)){

			particleList[particleNum]->Init(type);
			//this->GetNode()->AttachNode(particleList[particleNum]->GetNode());
			incrementNum(particleNum);
			particleList[particleNum]->Init(type);
			//this->GetNode()->AttachNode(particleList[particleNum]->GetNode());
			incrementNum(particleNum);
			particleList[particleNum]->Init(type);
			//this->GetNode()->AttachNode(particleList[particleNum]->GetNode());
			incrementNum(particleNum);
			particleList[particleNum]->Init(type);
			//this->GetNode()->AttachNode(particleList[particleNum]->GetNode());
			incrementNum(particleNum);;		
		} else if (type == SPARK && (activeCounter%17 == 0 || activeCounter%9 == 0)){
			particleList[particleNum]->Init(type);
			//this->GetNode()->AttachNode(particleList[particleNum]->GetNode());
			incrementNum(particleNum);
		} else if (type == SHIELD){
			particleList[particleNum]->Init(type);
			//particleList[particleNum]->Init(type);
			//particleList[particleNum]->InitTex(type);
			//this->GetNode()->AttachNode(particleList[particleNum]->GetNode());
			//incrementNum(particleNum);
		} else if((type == IMPACT) && activeCounter%20 == 0){
			particleList[particleNum]->Init(type);
			//this->GetNode()->AttachNode(particleList[particleNum]->GetNode());
			incrementNum(particleNum);
		} else if ((type == SHADOW) && activeCounter == 0){
			
			particleList[particleNum]->Init(type);
			//particleList[particleNum]->InitTex(type);
		} 
		
		
		
		++activeCounter;
	} 

	for(int i = 0; i<50;++i){
			particleList[i]->update(type,this->GetNode(),player1,player2);
			int a = 0;
		}

	int a = 0;
	this->GetNode()->SetLocalPosition(pos);
}

void ParticleEmitter::DeactivateEmitter(){
	isActive = false;
}

void ParticleEmitter::incrementNum(int &particleNum){
	if(particleNum < 49)
		++particleNum;
	else
		particleNum = 0;
}