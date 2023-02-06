#pragma once
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#ifndef PCH_H
#define PCH_H

#define _USE_MATH_DEFINES
#define NOMINMAX

//Need to turn this off because rapidjson does not like C++ 17... something something std::iterator not existing anymore.
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

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

//rapidjson
#include <stringbuffer.h>
#include <prettywriter.h>
#include <document.h>
typedef rapidjson::PrettyWriter<rapidjson::StringBuffer> Serializer;
typedef rapidjson::Document Deserializer;
typedef rapidjson::Value SerializedValue;

#include "Utility.h"

static unsigned short UI_TILE_SIZE = 16;

#endif