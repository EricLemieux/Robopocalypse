#pragma once

#include <vector>		//vector list
#include <glm\glm.hpp>	//Vectors and Matricies
#include <glm\ext.hpp>	//Quaternions

#define PI 3.14159265359

class Node
{
private:
	std::vector<Node *> children;
	Node *parent;

	glm::mat4 localTransform;
	glm::mat4 worldTransform;

	glm::mat4 rotX;
	glm::mat4 rotY;
	glm::mat4 rotZ;
	glm::mat4 translation;
	glm::mat4 frameTransform;
public:
	Node();

	//Translate the node in 3D space
	void translateNode(const glm::vec3 position);

	//Rotations around the x, y, and z axis
	void rotateNodeX(const float degree);
	void rotateNodeY(const float degree);
	void rotateNodeZ(const float degree);

	//Rotate using a quaternion.
	void setRotationQuat(glm::quat newQuat);

	//Attach a node as a child object
	void attachNode(Node *childNode);
	//Detach a node child object
	void detatchNode(Node *childNode);
	void Node::detatchNode(const int index);
	//Detach all of the node children objects
	void detatchAllNodes(void);

	//Update the node and its children
	void update();

	//return the world transformation matrix for the node
	//usefull for drawing what the node represents.
	glm::mat4 getWorldTransform(void);
};