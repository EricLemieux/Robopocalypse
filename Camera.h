#pragma once

#include <glm\glm.hpp>

#include "GameObject.h"
#include "SceneGraph.h"

class Camera : public GameObject
{
public:
	Camera();
	~Camera();

	void Update(glm::mat4 &viewMatrix);

	//////////
	//SETTERS
	//////////

	inline void SetTarget(glm::vec3 newTarget){ target = newTarget; }

	inline void SetUp(glm::vec3 newUp){ up = newUp; }

	//////////
	//GETTERS
	//////////

	inline glm::vec3 GetTarget(void){ return target; }

	inline glm::vec3 GetUp(void){ return up; }

private:
	glm::vec3 target;
	glm::vec3 up;
};