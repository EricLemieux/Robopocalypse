#include "GameObject.h"


GameObject::GameObject()
{
	sceneGraphObject = new Node;
	
	normalMapHandle = 1;/// loadTexture("Resources/NormalMaps/blank.jpg");

	target = new MorphTargets;
}


GameObject::~GameObject()
{
}

void GameObject::Render(void)
{
	VBO->ActivateAndRender();
}

glm::mat4 GameObject::UpdateModelViewProjection(glm::mat4 &projectionMat, glm::mat4 &viewMat)
{
	//Return the model view projection matrix
	return projectionMat * viewMat * sceneGraphObject->GetWorldTransform();
}

//////////
//Setters
//////////

//Set the position of the game object in world space
void GameObject::SetPosition(glm::vec3 newPos)
{
	sceneGraphObject->SetLocalPosition(newPos);
}
//Translate the position of the object
void GameObject::MovePosition(glm::vec3 translation)
{
	sceneGraphObject->TranslateNode(translation);
}

//Attach a child object in the scene graph
void GameObject::SetSceneGraphChild(Node *child)
{
	sceneGraphObject->AttachNode(child);
}

//////////
//ATTACHERS
//////////

//Attach a model to the game object
void GameObject::AttachModel(VertexBuffer *model)
{
	VBO = model;
}

void GameObject::AttachBones(char *meshFilePath, std::vector<glm::vec2> texcoords)
{
	if (VBO)
	{
		//std::vector<skinMesh> mesh = LoadSkinWeightsXML(meshFilePath);
		std::vector<skinMesh> mesh = LoadSkinWeightsIMG(meshFilePath, texcoords);

		std::vector<float> boneIDs;
		std::vector<float> boneWeights;
		for (unsigned int i = 0; i < mesh.size(); ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				boneIDs.push_back(mesh[i].boneInfluenceIDs[j]);
				boneWeights.push_back(mesh[i].weights[j]);
			}
		}
		VBO->AddBoneIndexes(&boneIDs[0], boneIDs.size());
		VBO->AddBoneWeights(&boneWeights[0], boneWeights.size());
	}
}

void GameObject::AttachTexture(GLuint tex)
{
	textureHandle = tex;
}

void GameObject::AttachNormalMap(GLuint nMap)
{
	normalMapHandle = nMap;
}

void GameObject::AttachCollisionBox(CollisionBox *box)
{
	collisionBox = box;
}


void GameObject::bvhTest(void)
{
	BVH temp = BVH("Resources\\BVH\\testbackup.bvh");
	temp.BuildSceneGraph(sceneGraphObject);
	temp.BuildMotionData();
	bvhFiles.push_back(temp);
	animations.SetAnimations(bvhFiles);
}