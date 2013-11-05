#include "Engine.h"

void checkLeftJoystick(int controllerNumber, Player &tempPlayer)
{
        float moveSpeed = 0.2f;

        //Basic structure
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Joystick::getAxisPosition(controllerNumber,sf::Joystick::Y) < -20)
        {
			tempPlayer.increaseVel(glm::vec3(0.0f, moveSpeed, 0.0f));
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Joystick::getAxisPosition(controllerNumber,sf::Joystick::Y) > 20)
        {
			tempPlayer.increaseVel(glm::vec3(0.0f, -moveSpeed, 0.0f));
        }

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Joystick::getAxisPosition(controllerNumber,sf::Joystick::X) < -20)
		{
			tempPlayer.increaseVel(glm::vec3(-moveSpeed, 0.0f, 0.0f));
		}

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Joystick::getAxisPosition(controllerNumber,sf::Joystick::X) > 20)
        {
			tempPlayer.increaseVel(glm::vec3(moveSpeed, 0.0f, 0.0f));
        }
}