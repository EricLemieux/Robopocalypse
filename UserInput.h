#ifndef USER_INPUT_H
#define USER_INPUT_H

#include <vector>

#include "Engine.h"

#include "Collisions.h"

void checkLeftJoystick(int controllerNumber, Player &tempPlayer);
void checkFaceButtons(void);
void checkTriggers(void);

void printButtons(void);

#endif