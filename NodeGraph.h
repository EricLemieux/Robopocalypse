#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include <glm\glm.hpp>
#include <vector>
#include "GLFW\glfw3.h"
//#include<SFML/OpenGL.hpp>


struct CollisionNode{
	glm::vec3 position;//position of the node
	int visited;//is the node occupied
	int row;//index number
	int column;
	float fromstart;//g score
	float starttogoal;//f score
	CollisionNode* camefrom;//parent
	int hasParent;//bool check
	
	std::vector<CollisionNode*> neighbours;//list of neighbours
};

class NodeGraph{
public:
	int size;//this value sets the size of the graph along both x and y
	std::vector<std::vector<CollisionNode*>> graph;

	NodeGraph();
	~NodeGraph();
	
	void init();
	void draw();
	void reset();
};


#endif