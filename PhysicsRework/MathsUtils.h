#pragma once
#include <random>
#include "Vector2.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define M_GOLDENRATIO ((1.0 + sqrt(5.0)) / 2.0)
#define M_TAU (2 * M_PI)

namespace MathsUtils
{
	////Works equally each direction from 0
	////e.g. GetRandomIntExcludingCentre(256, 128)
	////		can return between -256 -> -128 & 128 -> 256
	int GetRandomIntExcludingCentre(int upperBound, int lowerBound);
	float ConvertToRadians(float degrees);
	float ConvertToDegrees(float radians);

	////todo : make reference to store in point rather than returning
	Vector2 RotatePointAroundOriginRadians(Vector2 point, float rotation, Vector2 origin);
	Vector2 RotatePointAroundOriginDegrees(Vector2 point, float rotation, Vector2 origin);

	Vector2 LerpPoint(Vector2 start, Vector2 end, float time);

	////Returns -1 if val < 0, 1 if val > 0, 0 if val == 0
	int Sign(int val);
	float Dot(Vector2 a, Vector2 b);

	float GetAngleBetweenTwoVectorsDegrees(const Vector2& a, const Vector2& b);
	float GetAngleBetweenTwoVectors(const Vector2& a, const Vector2& b);

	float PointDistanceToLine(Point point, Line line);
	float PointDistanceToLineSigned(Point point, Line line);
	bool  IsPointAboveLine(const Point& point, const Line& line);

	Vector2 CalculateIntersectionPointOfTwoLines(const Line& lineA, const Line& lineB);
}
