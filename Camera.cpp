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

	camPos.z = 1.2f * playerToCenter * tanf(PI/5);
}

void Camera::update(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f,1.f,1.f,1000.f);
	gluLookAt(playerCenter.x,camPos.y,camPos.z,playerCenter.x,targetPos.y,targetPos.z,orientation.x,orientation.y,orientation.z);
	//gluLookAt(0,0,0,0,0,-20,0,1,0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}