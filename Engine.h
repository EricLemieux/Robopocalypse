#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include<SFML/OpenGL.hpp>

#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <math.h>

#define PI  3.14159265f
#define DTR 0.01745329f
#define RTD 57.2957795f

#define WINDOW_SCREEN_WIDTH 1400
#define WINDOW_SCREEN_HEIGHT 800
#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 800
#define HUD_HEIGHT 100

#include "OBJModel.h"
#include "Animation.h"
#include "AnimationMath.h"
#include "LoadAssets.h"
#include "Path.h"

#include "Player.h"
#include "Camera.h"

//#include "Collisions.h"
#include "UserInput.h"

#endif