#include "SceneGraph.h"

Node::Node()
{
	translation = glm::mat4(1.0f);
	rotX		= glm::mat4(1.0f);
	rotY		= glm::mat4(1.0f);
	rotZ		= glm::mat4(1.0f);
}

//Translate the node in 3D space
void Node::translateNode(const glm::vec3 position)
{
	translation[3] = glm::vec4(position, 1.0f);
}

//Rotations around the x, y, and z axis
void Node::rotateNodeX(const float degree)
{
	rotX[1] = glm::vec4(0, cos(degree * PI/180), sin(degree * PI/180), 0);
	rotX[2] = glm::vec4(0, -sin(degree * PI/180), cos(degree * PI/180), 0);
}
void Node::rotateNodeY(const float degree)
{
	rotY[0] = glm::vec4(cos(degree * PI/180), 0, -sin(degree * PI/180), 0);
	rotY[2] = glm::vec4(sin(degree * PI/180), 0, cos(degree * PI/180), 0);
}
void Node::rotateNodeZ(const float degree)
{
	rotZ[0] = glm::vec4(cos(degree * PI/180), sin(degree * PI/180), 0, 0);
	rotZ[1] = glm::vec4(-sin(degree * PI/180), cos(degree * PI/180), 0, 0);
}

//Rotate using a quaternion
void Node::setRotationQuat(glm::quat newQuat)
{
	frameTransform = glm::toMat4(newQuat);
}

//Attach a node as a child object
void Node::attachNode(Node *childNode)
{
	children.push_back(childNode);
}
//Detach a node child object
void Node::detatchNode(Node *childNode)
{
	for(int i = 0; i < children.size(); ++i)
	{
		if(children[i] == childNode)
		{
			detatchNode(i);
		}
	}
}
void Node::detatchNode(const int index)
{
	children[index]			= 0;
	children[index]->parent = 0;
}
//Detach all of the node children objects
void Node::detatchAllNodes(void)
{
	children.clear();
}

//Update the node and its children
void Node::update()
{
	localTransform = translation * rotX * rotY * rotZ;
	worldTransform = parent->worldTransform * localTransform;

	//Loop through all children and update
	for(int i = 0; i < children.size(); ++i)
	{
		children[i]->update();
	}
}

//return the world transformation matrix for the node
//usefull for drawing what the node represents.
glm::mat4 Node::getWorldTransform(void)
{
	return worldTransform;
}