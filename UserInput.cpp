#include "Engine.h"

void checkLeftJoystick(int controllerNumber, Player &tempPlayer)
{
        float moveSpeed = 0.2f;
		bool action = false;
        //Holdable actions (movement)
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Joystick::getAxisPosition(controllerNumber,sf::Joystick::Y) < -20)
        {
			tempPlayer.increaseVel(glm::vec3(0.0f, moveSpeed, 0.0f));
			action = true;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Joystick::getAxisPosition(controllerNumber,sf::Joystick::Y) > 20)
        {
			tempPlayer.increaseVel(glm::vec3(0.0f, -moveSpeed, 0.0f));
			action = true;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Joystick::getAxisPosition(controllerNumber,sf::Joystick::X) < -20)
		{
			tempPlayer.updateAction(2);
			action = true;
		}

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Joystick::getAxisPosition(controllerNumber,sf::Joystick::X) > 20)
        {
			tempPlayer.updateAction(1);
			action = true;
		}


		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Joystick::getAxisPosition(controllerNumber,sf::Joystick::Y) < -20)
        {
			tempPlayer.updateAction(5);
			action = true;
		} 
		
		if(!action){
			tempPlayer.updateAction(0);
		}
}