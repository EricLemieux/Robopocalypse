#include "Engine.h"

Camera::Camera(){
	camPos.x = 0.f;
	camPos.y = 0.f;
	camPos.z = 0.f;

	targetPos.x = 0.f;
	targetPos.y = 0.f;
	targetPos.z = -50.f;

	orientation.x = 0.f;
	orientation.y = 1.f;
	orientation.z = 0.f;

	playerCenter.x = 0.f;
	playerCenter.y = 0.f;
	playerCenter.z = -20.f;

}
Camera::~Camera(){}

void Camera::setFocus(Player player1, Player player2){
	float playerToCenter;
	playerCenter.x = (player1.getPosX() + player2.getPosX()) / 2;
	playerCenter.y = (player1.getPosY() + player2.getPosY())/2;
	playerCenter.z = (player1.getPosZ() + player2.getPosZ())/2;

	playerToCenter = sqrt(pow(player1.getPosX() - playerCenter.x,2)+pow(player1.getPosY() - playerCenter.y,2)+pow(player1.getPosZ() - playerCenter.z,2));

	camPos.x = targetPos.x = playerCenter.x;
	camPos.y = targetPos.y = 40;
	camPos.z = 1.2f * playerToCenter * tanf(PI/5);
}

void Camera::setFocus(Player player1)
{
	//Camera is looking at the player
	targetPos = player1.getPos();
	
	//Update the camera's position
	camPos.x = player1.getPos().x + 10 * player1.getFaceDirection();
	camPos.y = player1.getPos().y + 1;
	camPos.z = player1.getPos().z;
}

void Camera::setPos(glm::vec3 pos1, glm::vec3 pos2, float dt, float maxT)
{
	//Set the position
	dt *= 1/maxT;
	camPos = LERP(pos1,pos2,dt);
}

void Camera::setTarget(glm::quat q1, glm::quat q2, float dt, float maxT)
{
	//Set the target
	dt *= 1/maxT;
	glm::quat qf = SLERP(q1, q2, dt);
	
	targetPos.x = qf.x;
	targetPos.y = qf.y;
	targetPos.z = qf.z;
}

void Camera::setTarget(glm::vec3 target)
{
	targetPos = target;
}

void Camera::update(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f,1.f,1.f,1000.f);
	gluLookAt(camPos.x, camPos.y, camPos.z, targetPos.x, targetPos.y, targetPos.z, orientation.x, orientation.y, orientation.z);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}