//This class is for the camera, where it is in the world and what it is looking at.

#ifndef CAMERA_H
#define CAMERA_H

#include "AnimationMath.h"

class Camera{
public:
	Camera();
	~Camera();

	//compute camera and target position
	void setFocus(Player player1, Player player2);							//Focus between the to characters
	void setFocus(Player player1);											//Target the one player in a face on shot.

	//Set the position of the camera
	void setPos(glm::vec3 pos1, glm::vec3 pos2, float dt, float maxT);		//Animated camera for cutscenes, used with LERP

	//Set where the camera is looking
	void setTarget(glm::quat q1, glm::quat q2, float dt, float maxT);		//Setting the target for the camera using SLERP
	void setTarget(glm::vec3 target);										//Setting the target to a specific position

	//gluLookAt in here
	void update();

	//get/set time
	inline void		setTime(float t){ time = t; }
	inline float	getTime(void)	{ return time; }

	//get the camera's current position
	inline glm::vec3 getCamPos(void){ return camPos; }

private:
	glm::vec3 camPos;
	glm::vec3 targetPos;
	glm::vec3 orientation;
	glm::vec3 playerCenter;

	float time;
};

#endif