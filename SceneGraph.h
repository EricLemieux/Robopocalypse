#pragma once

//TODO
//This needs rotations

#define PI 3.14159265359

#include <vector>		//vector list
#include <glm\glm.hpp>	//Vectors and Matricies
#include <glm\ext.hpp>	//Quaternions

class Node
{
private:
	std::vector<Node *> children;
	Node *parent;

	glm::mat4 localTransform;
	glm::mat4 worldTransform;

	glm::mat4 rotation;
	glm::mat4 translation;
	glm::mat4 frameTransform;
public:
	Node(void);
	~Node(void);

	//////////
	//SETTERS
	//////////

	//Translate the node in 3D space
	void TranslateNode(const glm::vec3 position);
	void SetLocalPosition(const glm::vec3 position);
	

	//////////
	//GETTERS
	//////////

	//return the world transformation matrix for the node
	//usefull for drawing what the node represents.
	glm::mat4 GetWorldTransform(void);

	glm::vec3 GetWorldPosition(void);

	//////////
	//ATTACH / DETACH
	//////////

	//Attach a node as a child object
	void AttachNode(Node *childNode);
	//Detach a node child object
	void DetatchNode(Node *childNode);
	void DetatchNode(const int index);
	//Detach all of the node children objects
	void DetatchAllNodes(void);

	//////////
	//UPDATE
	//////////

	//Update the node and its children
	void Update();
};