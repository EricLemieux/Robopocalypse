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

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Joystick::getAxisPosition(controllerNumber,sf::Joystick::X) < -20)
		{
			tempPlayer.updateAction(4);
			action = true;
		}

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::E) || sf::Joystick::getAxisPosition(controllerNumber,sf::Joystick::X) > 20)
        {
			tempPlayer.updateAction(3);
			action = true;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Joystick::getAxisPosition(controllerNumber,sf::Joystick::X) > 20)
        {
			tempPlayer.updateAction(5);
			action = true;
		} 

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::J) || sf::Joystick::getAxisPosition(controllerNumber,sf::Joystick::X) > 20)
        {
			tempPlayer.updateAction(6);
			action = true;
		} 

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::K) || sf::Joystick::getAxisPosition(controllerNumber,sf::Joystick::X) > 20)
        {
			tempPlayer.updateAction(7);
			action = true;
		} 


		if(!action){
			tempPlayer.updateAction(0);
		}
}