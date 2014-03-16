#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "SceneGraph.h"

enum 
{
	BVH_H_LOAD_ERROR = 0,
	BVH_H_LOAD_FINE,
	BVH_F_LOAD_ERROR,
	BVH_F_LOAD_FINE,
};

struct Joint
{
	unsigned int ID;
	char* name;

	Node* node;
	int numChanels;
};

class BVH
{
public:
	BVH();
	BVH(char* filePath);
	~BVH();

	void LoadFile(char* filePath);

	int BuildSceneGraph();

private:
	char* filePath;

	Node *rootNode;
	std::vector<Joint> nodeTree;
};