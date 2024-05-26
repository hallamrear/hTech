#pragma once
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#ifndef PCH_H
#define PCH_H

#define _USE_MATH_DEFINES
#define NOMINMAX

//Need to turn this off because rapidjson does not like C++ 17... something something std::iterator not existing anymore.
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

//SDL
#include <External/SDL.h>

//ImGui
#include <External/IMGUI.h>

//STB

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
#include <memory>
#include <vector>
#include <filesystem>

#include <System/Utility.h>

static unsigned short UI_TILE_SIZE = 16;

#endif