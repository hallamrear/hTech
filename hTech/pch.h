#pragma once
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#ifndef PCH_H
#define PCH_H

#define _USE_MATH_DEFINES
#define NOMINMAX

//Need to turn this off because rapidjson does not like C++ 17... something something std::iterator not existing anymore.
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

#define assert_now assert(false)
#define FUNC_AND_LINE "[" << __func__ << "][Line " << __LINE__ << "]"
#define assert_func_not_implemented assert_now; std::cout << FUNC_AND_LINE << "not implemented." << std::endl;

//SDL
#include <External/SDL.h>

//ImGui
#include <External/IMGUI.h>

//C++ Libaries
#include <algorithm>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>
#include <mmsystem.h>
#include <random>
#include <string>
#include <vector>
#include <Windows.h>

#include <System/Utility.h>

static unsigned short UI_TILE_SIZE = 16;

#endif