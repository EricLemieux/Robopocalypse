#include "Camera.h"


Camera::Camera()
{
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	target = glm::vec3(0.0f, 0.0f, 0.0f);
}


Camera::~Camera()
{
}

void Camera::Update(glm::mat4 *viewMatrix)
{
	viewMatrix = &glm::lookAt(GetNode()->GetWorldPosition(), target, up);
}