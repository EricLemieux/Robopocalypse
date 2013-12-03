#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include "glm\glm.hpp"
#include <vector>
#include<SFML/OpenGL.hpp>

struct CollisionNode{
	glm::vec3 position;
	int visited;
	int row;
	int column;
	float fromstart;
	float starttogoal;
	CollisionNode* camefrom;
	int hasParent;
	
	std::vector<CollisionNode*> neighbours;
};

class NodeGraph{
public:
	int size;
	std::vector<std::vector<CollisionNode*>> graph;

	NodeGraph();
	~NodeGraph();
	
	void init();
	void draw();
	void reset();
};


#endif