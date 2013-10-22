#pragma once

#include <SFML\Window.hpp>
#include <iostream>

//#include "Vec3.h"

//#include "LoadAssets.h"
//#include "obj.h"
//#include "Camera.h"

#include "Engine.h"


//void checkInput(obj objectToCheck);
//Camera checkRightJoystick(Camera cameraToAim);
void checkLeftJoystick(int num, Player &tempPlayer);
void checkFaceButtons(void);
void checkTriggers(void);

void printButtons(void);