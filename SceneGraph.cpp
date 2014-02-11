#include "SceneGraph.h"

Node::Node()
{
	parent = 0;

	glm::mat4 localTransform	= glm::mat4(1);
	glm::mat4 worldTransform	= glm::mat4(1);
								
	glm::mat4 rotation			= glm::mat4(1);
	glm::mat4 translation		= glm::mat4(1);
	glm::mat4 frameTransform	= glm::mat4(1);
}
Node::~Node()
{
}

//Translate the node in 3D space
void Node::TranslateNode(const glm::vec3 position)
{
	translation[3] += glm::vec4(position, 1.0f);
}
void Node::SetLocalPosition(const glm::vec3 position)
{
	translation[3] = glm::vec4(position, 1.0f);
}

//Attach a node as a child object
void Node::AttachNode(Node *childNode)
{
	childNode->parent = this;
	children.push_back(childNode);
}
//Detach a node child object
void Node::DetatchNode(Node *childNode)
{
	for (int i = 0; i < children.size(); ++i)
	{
		if (children[i] == childNode)
		{
			DetatchNode(i);
		}
	}
}
void Node::DetatchNode(const int index)
{
	children[index] = 0;
	children[index]->parent = 0;
}
//Detach all of the node children objects
void Node::DetatchAllNodes(void)
{
	children.clear();
}

//Update the node and its children
void Node::Update()
{
	localTransform = translation * rotation;
	worldTransform = this->parent->worldTransform * this->localTransform;

	//Loop through all children and update
	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->Update();
	}
}

//return the world transformation matrix for the node
//usefull for drawing what the node represents.
glm::mat4 Node::GetWorldTransform(void)
{
	return worldTransform;
}