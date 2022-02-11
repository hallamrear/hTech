#pragma once
#ifndef PCH_H
#define PCH_H

#ifdef HTECH_EXPORTS
#define HTECH_API __declspec(dllexport)
#else
#define HTECH_API __declspec(dllimport)
#endif

#define _USE_MATH_DEFINES
#define NOMINMAX

//SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_audio.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

//C++ Libaries
#include <Windows.h>
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <random>
#include <cmath>
#include <mmsystem.h>
#include <cassert>

#include "Helper.h"
#include "Settings.h"

static float TEST_ZOOM = 1.0f;

#endif