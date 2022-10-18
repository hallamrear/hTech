#include "pch.h"
#include "Helper.h"

//Works equally each direction from 0
//e.g. GetRandomIntExcludingCentre(256, 128)
//		can return between -256 -> -128 & 128 -> 256
int MathsHelp::GetRandomIntExcludingCentre(int upperBound, int lowerBound)
{
	int value = rand() % upperBound;

	if (value < lowerBound)
		value = (upperBound - value) * -1;

	return value;
}

float MathsHelp::ConvertToRadians(float degrees)
{
	return degrees * (float)(M_PI / 180.0);
}

float MathsHelp::ConvertToDegrees(float radians)
{
	return radians * (float)(180.0 / M_PI);
}

Vector2 MathsHelp::RotatePointAroundOriginRadians(Vector2 point, float rotation, Vector2 origin)
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
	Vector2 value;
	value.X = xnew + origin.X;
	value.Y = ynew + origin.Y;
	return value;
}

Vector2 MathsHelp::RotatePointAroundOriginDegrees(Vector2 point, float rotation, Vector2 origin)
{
	return RotatePointAroundOriginRadians(point, MathsHelp::ConvertToRadians(rotation), origin);
}

Vector2 MathsHelp::LerpPoint(Vector2 start, Vector2 end, float time)
{
	return (start * (1 - time) + end * time);
}

//Returns -1 if val < 0, 1 if val > 0, 0 if val == 0
int MathsHelp::Sign(int val)
{
	return ((0) < val) - (val < (0));
}

float MathsHelp::Dot(Vector2 a, Vector2 b)
{
	return (a.X * b.X) + (a.Y * b.Y);
}