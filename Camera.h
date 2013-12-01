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
	void setFocus(glm::vec3 pos1, glm::vec3 pos2, float dt, float maxT);	//Animated camera for cutscenes, ment to be used with LERP

	//gluLookAt in here
	void update();

	inline void		setTime(float t){ time = t; }
	inline float	getTime(void)	{ return time; }

	inline glm::vec3 getCamPos(void){ return camPos; }

private:
	glm::vec3 camPos;
	glm::vec3 targetPos;
	glm::vec3 orientation;
	glm::vec3 playerCenter;

	float time;
};

#endif