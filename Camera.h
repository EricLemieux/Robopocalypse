#ifndef CAMERA_H
#define CAMERA_H

#include "Engine.h"

class Camera{
public:
	Camera();
	~Camera();

	//compute camera and target position
	void setFocus(Player player1, Player player2);

	//gluLookAt in here
	void update();

private:
	sf::Vector3f camPos;
	sf::Vector3f targetPos;
	sf::Vector3f orientation;
	sf::Vector3f playerCenter;
};

#endif