#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#ifndef PCH_H
#define PCH_H

#ifndef HTECH_FUNCTION_EXPORT
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)
#endif

#define _USE_MATH_DEFINES
#define NOMINMAX

//SDL
#include <SDL.h>
#include <SDL_audio.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

//STB //TODO : if want to use image writing, like screenshotting uncomment this and use like stb_image
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include <stb_image_write.h>

//C++ Libaries
#include <Windows.h>
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <random>
#include <cmath>
#include <mmsystem.h>
#include <algorithm>
#include <cassert>

#include "Helper.h"
#include "Settings.h"

static float TEST_ZOOM = 1.0f;
static unsigned short UI_TILE_SIZE = 16;

#endif