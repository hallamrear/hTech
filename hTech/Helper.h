#pragma once
#include <random>
#include "Vector2.h"
#include "Vector3.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace HelperFunctions
{
	////Works equally each direction from 0
	////e.g. GetRandomIntExcludingCentre(256, 128)
	////		can return between -256 -> -128 & 128 -> 256
	int GetRandomIntExcludingCentre(int upperBound, int lowerBound);
	float ConvertToRadians(float degrees);
	float ConvertToDegrees(float radians);

	////todo : make reference to store in point rather than returning
	Vector2f RotatePointAroundOriginRadians(Vector2f point, float rotation, Vector2f origin);
	Vector2f RotatePointAroundOriginDegrees(Vector2f point, float rotation, Vector2f origin);
	Vector2f LerpPoint(Vector2f start, Vector2f end, float time);

	////Returns -1 if val < 0, 1 if val > 0, 0 if val == 0
	int Sign(int val);
	float Dot(Vector2f a, Vector2f b);
}