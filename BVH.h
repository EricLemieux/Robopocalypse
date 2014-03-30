#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "SceneGraph.h"

enum 
{
	BVH_H_LOAD_ERROR = 0,
	BVH_H_LOAD_FINE,
	BVH_M_LOAD_ERROR,
	BVH_M_LOAD_FINE,
};

struct Joint
{
	unsigned int ID;
	char* name;

	Node* node;
	int numChannels;

	std::vector<glm::vec3> positionChanges;
	std::vector<glm::vec3> rotationChanges;
	glm::mat4 offset;
};

class BVH
{
public:
	BVH();
	BVH(char* filePath);
	~BVH();

	void LoadFile(char* filePath);

	int BuildSceneGraph(Node *parent);
	int BuildMotionData(void);
	
	//////////
	//GETTERS
	//////////

	inline unsigned int GetNumFrames(void){ return numFrames; }
	inline float GetFrameTime(void){ return frameTime; }

	inline Node* GetRootNode(void){	return rootNode; }
	inline std::vector<Joint> GetNodeTree(void){ return nodeTree; }

private:
	char* filePath;

	Node *rootNode;
	std::vector<Joint> nodeTree;

	unsigned int numFrames;
	float frameTime;
};