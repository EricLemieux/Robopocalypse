#include "Engine.h"

void checkLeftJoystick(int num, Player &tempPlayer)
{
        float moveSpeed = 0.2f;

        //Basic structure
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