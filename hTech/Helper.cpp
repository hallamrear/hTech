#include "pch.h"
#include "Helper.h"

//Works equally each direction from 0
//e.g. GetRandomIntExcludingCentre(256, 128)
//		can return between -256 -> -128 & 128 -> 256
int HelperFunctions::GetRandomIntExcludingCentre(int upperBound, int lowerBound)
{
	int value = rand() % upperBound;

	if (value < lowerBound)
		value = (upperBound - value) * -1;

	return value;
}

float HelperFunctions::ConvertToRadians(float degrees)
{
	return degrees * (float)(M_PI / 180.0);
}

float HelperFunctions::ConvertToDegrees(float radians)
{
	return radians * (float)(180.0 / M_PI);
}

//todo : make reference to store in point rather than returning
Vector2f HelperFunctions::RotatePointAroundOriginRadians(Vector2f point, float rotation, Vector2f origin)
{
	float s = sin(rotation);
	float c = cos(rotation);

	// translate point back to origin:
	point.X -= origin.X;
	point.Y -= origin.Y;

	// rotate point
	float xnew = point.X * c - point.Y * s;
	float ynew = point.X * s + point.Y * c;

	// translate point back:
	Vector2f value;
	value.X = xnew + origin.X;
	value.Y = ynew + origin.Y;
	return value;
}

Vector2f HelperFunctions::RotatePointAroundOriginDegrees(Vector2f point, float rotation, Vector2f origin)
{
	return RotatePointAroundOriginRadians(point, HelperFunctions::ConvertToRadians(rotation), origin);
}

Vector2f HelperFunctions::LerpPoint(Vector2f start, Vector2f end, float time)
{
	return (start * (1 - time) + end * time);
}

//Returns -1 if val < 0, 1 if val > 0, 0 if val == 0
int HelperFunctions::Sign(int val)
{
	return ((0) < val) - (val < (0));
}

float HelperFunctions::Dot(Vector2f a, Vector2f b)
{
	return (a.X * b.X) + (a.Y * b.Y);
}