//This is used to store information that is being used by several other files, such as the window height or other header files

#ifndef ENGINE_H
#define ENGINE_H

#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "GL\glew.h"
#include "GLFW\glfw3.h"

#undef _UNICODE
#include <IL\il.h>
#include <IL\ilu.h>
#include <IL\ilut.h>

//SFML window and graphics used to be here

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <math.h>

//#define PI  3.14159265f
#define DTR 0.01745329f
#define RTD 57.2957795f

#define WINDOW_SCREEN_WIDTH 1400
#define WINDOW_SCREEN_HEIGHT 800
#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 800
#define HUD_HEIGHT 100

#include "Particles.h"
#include "OBJModel.h"
#include "AnimationMath.h"
#include "LoadAssets.h"
#include "Path.h"
#include "NodeGraph.h"

#include "Player.h"
#include "Camera.h"

#include "Collisions.h"
#include "UserInput.h"

#endif