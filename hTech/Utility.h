#pragma once
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include <random>
#include "Vector2.h"
#include "Line.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define M_GOLDENRATIO ((1.0 + sqrt(5.0)) / 2.0)
#define M_TAU (2 * M_PI)

namespace Utils
{
	namespace Maths
	{
		struct ClosestPointDistanceResult
		{
			Vector2 ClosestPoint;
			float DistanceSquared;
		};

		////Works equally each direction from 0
		////e.g. GetRandomIntExcludingCentre(256, 128)
		////		can return between -256 -> -128 & 128 -> 256
		HTECH_FUNCTION_EXPORT int GetRandomIntExcludingCentre(int upperBound, int lowerBound);
		HTECH_FUNCTION_EXPORT float ConvertToRadians(float degrees);
		HTECH_FUNCTION_EXPORT float ConvertToDegrees(float radians);

		////todo : make reference to store in point rather than returning
		HTECH_FUNCTION_EXPORT Vector2 RotatePointAroundOriginRadians(Vector2 point, float rotation, Vector2 origin);
		HTECH_FUNCTION_EXPORT Vector2 RotatePointAroundOriginDegrees(Vector2 point, float rotation, Vector2 origin);

		HTECH_FUNCTION_EXPORT Vector2 LerpPoint(Vector2 start, Vector2 end, float time);

		////Returns -1 if val < 0, 1 if val > 0, 0 if val == 0
		HTECH_FUNCTION_EXPORT int Sign(int val);
		HTECH_FUNCTION_EXPORT float Dot(Vector2 a, Vector2 b);

		HTECH_FUNCTION_EXPORT float	CrossMagnitude(const Vector2& a, const Vector2& b);
		HTECH_FUNCTION_EXPORT Vector2 Cross(const Vector2& a, float s);
		HTECH_FUNCTION_EXPORT Vector2 Cross(float s, const Vector2& a);

		HTECH_FUNCTION_EXPORT float GetAngleBetweenTwoVectorsDegrees(const Vector2& a, const Vector2& b);
		HTECH_FUNCTION_EXPORT float GetAngleBetweenTwoVectors(const Vector2& a, const Vector2& b);

		HTECH_FUNCTION_EXPORT float PointDistanceToLine(Point point, Line line);
		HTECH_FUNCTION_EXPORT float PointDistanceToLineSigned(Point point, Line line);
		HTECH_FUNCTION_EXPORT bool  IsPointAboveLine(const Point& point, const Line& line);

		HTECH_FUNCTION_EXPORT ClosestPointDistanceResult FindClosestPointOnLine(const Vector2& point, const Vector2& A, const Vector2& B);
		HTECH_FUNCTION_EXPORT ClosestPointDistanceResult FindClosestPointOnLine(const Vector2& point, const Line& AB);

		HTECH_FUNCTION_EXPORT float DistanceSquared(const Vector2& a, const Vector2& b);
		HTECH_FUNCTION_EXPORT float Distance(const Vector2& a, const Vector2& b);

		/// <summary>
		/// Due to floating point inprefections, you can have two values be extremely close to eachother without actually being equal.
		/// This function checks if they are within an amount of each other.
		/// </summary>
		/// <param name="A"></param>
		/// <param name="B"></param>
		/// <param name="tolerence">The allowed gap between A and B for the check to pass.</param>
		/// <returns></returns>
		HTECH_FUNCTION_EXPORT bool AreFloatingPointsWithinTolerence(const float& A, const float& B, const float& tolerance);
		HTECH_FUNCTION_EXPORT bool AreTwoPointsWithinTolerence(const Vector2& A, const Vector2& B, const float& tolerance);
		HTECH_FUNCTION_EXPORT Vector2 TripleProduct(const Vector2& a, const Vector2& b, const Vector2& c);
	}
};