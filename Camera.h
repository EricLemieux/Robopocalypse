#ifndef CAMERA_H
#define CAMERA_H

class Camera{
public:
	Camera();
	~Camera();

	//compute camera and target position
	void setFocus(Player player1, Player player2);

	//gluLookAt in here
	void update();

private:
	glm::vec3 camPos;
	glm::vec3 targetPos;
	glm::vec3 orientation;
	glm::vec3 playerCenter;
};

#endif