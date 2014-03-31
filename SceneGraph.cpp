#include "SceneGraph.h"

Node::Node()
{
	parent = 0;

	localTransform	= glm::mat4(1.0f);
	worldTransform	= glm::mat4(1.0f);
					
	scale			= glm::mat4(1.0f);
	rotation		= glm::mat4(1.0f);
	translation		= glm::mat4(1.0f);
	frameTransform	= glm::mat4(1.0f);

	ID = 0;
}
Node::~Node()
{
}

//Translate the node in 3D space
void Node::TranslateNode(const glm::vec3 position)
{
	translation[3] += glm::vec4(position, 0.0f);
}
void Node::SetLocalPosition(const glm::vec3 position)
{
	translation[3] = glm::vec4(position, 1.0f);
}

//Set the local rotation
void Node::SetRotation(glm::mat4 newRot)
{
	rotation = newRot;
}
void Node::SetRotation(glm::vec3 rot)
{
	//Convert to radians
	rot = rot * glm::vec3(PI / 180.0f);
	rotation = glm::yawPitchRoll(rot.x, rot.y, rot.z);
}

//Set the local scale
void Node::SetScale(glm::mat4 newScale)
{
	scale = newScale;
}
void Node::SetScale(glm::vec3 newScale)
{
	glm::vec4 a[4];

	a[0].x = newScale.x;
	a[1].y = newScale.y;
	a[2].z = newScale.z;
	a[3].w = 1.0f;

	scale = glm::mat4(a[0], a[1], a[2], a[3]);
}

void Node::SetID(unsigned int newID)
{
	ID = newID;
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
	children[index]->parent = 0;
	children[index] = 0;
	children.erase(children.begin() + index);
}
//Detach all of the node children objects
void Node::DetatchAllNodes(void)
{
	children.clear();
}

//Update the node and its children
void Node::Update()
{
	UpdateLocalPosition();
	worldTransform = this->parent->worldTransform * this->localTransform;

	//Loop through all children and update
	for (int i = 0; i < children.size(); ++i)
	{
		children[i]->Update();
	}
}

void Node::UpdateLocalPosition(void)
{
	localTransform = translation * rotation * scale;
}

//return the world transformation matrix for the node
//usefull for drawing what the node represents.
glm::mat4 Node::GetWorldTransform(void)
{
	return worldTransform;
}

//Return the world position
glm::vec3 Node::GetWorldPosition(void)
{
	return glm::vec3(worldTransform[3].x, worldTransform[3].y, worldTransform[3].z);
}

glm::vec3 Node::GetScale(void)
{
	return glm::vec3(scale[0].x, scale[1].y, scale[2].z);
}

glm::vec3 Node::GetLocalPosition(void)
{
	return glm::vec3(localTransform[3].x, localTransform[3].y, localTransform[3].z);
}

glm::mat4 Node::GetLocalTransform(void)
{
	return localTransform;
}

Node* Node::GetParent(void)
{
	return parent;
}

unsigned int Node::GetID(void)
{
	return ID;
}