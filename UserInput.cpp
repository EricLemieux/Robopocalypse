#include "UserInput.h"

//void checkInput(obj objectToCheck)
//{
//	sf::Joystick::update();
//
//}


void checkLeftJoystick(int num, Player &tempPlayer)
{
	float moveSpeed = 0.2f;

	//Basic structure
	//objectToMove.setPos(objectToMove.getPosX(), objectToMove.getPosY(), objectToMove.getPosZ());

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Joystick::getAxisPosition(num,sf::Joystick::Y) < -20)
	{
		//TODO
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Joystick::getAxisPosition(num,sf::Joystick::Y) > 20)
	{
		//TODO
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Joystick::getAxisPosition(num,sf::Joystick::X) < -20)
	{
		tempPlayer.decreaseVelX(moveSpeed);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Joystick::getAxisPosition(num,sf::Joystick::X) > 20)
	{
		tempPlayer.increaseVelX(moveSpeed);
	}
}

/*//TODO
Camera checkRightJoystick(Camera cameraToAim)
{
	float turnSpeed = 0.2f;

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Joystick::getAxisPosition(0,sf::Joystick::R) < -20)
	{
		//cameraToAim.setTarget(cameraToAim.getTarget().x, cameraToAim.getTarget().y + turnSpeed, cameraToAim.getTarget().z);
		cameraToAim.rotateX(turnSpeed);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Joystick::getAxisPosition(0,sf::Joystick::R) > 20)
	{
		cameraToAim.rotateX(-turnSpeed);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Joystick::getAxisPosition(0,sf::Joystick::U) < -20)
	{
		cameraToAim.rotateY(turnSpeed);
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Joystick::getAxisPosition(0,sf::Joystick::U) > 20)
	{
		cameraToAim.rotateY(-turnSpeed);
	}

	return cameraToAim;
}

//TODO
void checkFaceButtons(void)
{
	//
	if(sf::Joystick::isButtonPressed(0,6))
	{
		exit(1);
	}
}*/