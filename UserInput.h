#ifndef USER_INPUT_H
#define USER_INPUT_H

#include "Engine.h"

void checkLeftJoystick(int num, Player &tempPlayer);
void checkFaceButtons(void);
void checkTriggers(void);

void printButtons(void);

#endif