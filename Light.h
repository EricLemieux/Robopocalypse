#pragma once

#include "GameObject.h"

class Light : public GameObject
{
public:
	Light();
	~Light();

	inline glm::vec3 GetColour(void){ return colour; }

	void SetColour(glm::vec3 newColour);

private:
	glm::vec3 colour;
};