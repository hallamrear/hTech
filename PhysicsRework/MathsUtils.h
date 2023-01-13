#pragma once
#include <random>
#include "Vector2.h"
#include "Vector3.h"
#include "RenderLine.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define M_GOLDENRATIO ((1.0 + sqrt(5.0)) / 2.0)
#define M_TAU (2 * M_PI)

static Vector2 WorldToScreen(Vector2 worldPosition)
{
	return Vector2(400 + worldPosition.X, 300 - worldPosition.Y);
}

static Vector2 ScreenToWorld(Vector2 screenPosition)
{
	return Vector2(screenPosition.X - 400, screenPosition.Y + 300);
}

static void DrawLineToScreen(SDL_Renderer* renderer, Point pointA, Point pointB)
{
	Vector2 p1 = pointA, p2 = pointB;
	p1 = WorldToScreen(p1);
	p2 = WorldToScreen(p2);
	SDL_RenderDrawLine(renderer, (int)p1.X, (int)p1.Y, (int)p2.X, (int)p2.Y);
}

static void DrawPointToScreen(SDL_Renderer* renderer, Point pointA)
{
	Vector2 p1 = pointA;
	p1 = WorldToScreen(p1);
	p1.X--; p1.Y--;

	SDL_Rect rect{ p1.X, p1.Y, 2, 2 };
	SDL_RenderFillRect(renderer, &rect);
}

namespace MathsUtils
{
	struct ClosestPointDistanceResult
	{
		Vector2 ClosestPoint;
		float DistanceSquared;
	};

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

	float	CrossMagnitude(const Vector2& a, const Vector2& b);
	Vector2 Cross(const Vector2& a, float s);
	Vector2 Cross(float s, const Vector2& a);

	float GetAngleBetweenTwoVectorsDegrees(const Vector2& a, const Vector2& b);
	float GetAngleBetweenTwoVectors(const Vector2& a, const Vector2& b);

	float PointDistanceToLine(Point point, Line line);
	float PointDistanceToLineSigned(Point point, Line line);
	bool  IsPointAboveLine(const Point& point, const Line& line);

	ClosestPointDistanceResult FindClosestPointOnLine(const Vector2& point, const Vector2& A, const Vector2& B);
	ClosestPointDistanceResult FindClosestPointOnLine(const Vector2& point, const Line& AB);

	float DistanceSquared(const Vector2& a, const Vector2& b);
	float Distance(const Vector2& a, const Vector2& b);

	/// <summary>
	/// Due to floating point inprefections, you can have two values be extremely close to eachother without actually being equal.
	/// This function checks if they are within an amount of each other.
	/// </summary>
	/// <param name="A"></param>
	/// <param name="B"></param>
	/// <param name="tolerence">The allowed gap between A and B for the check to pass.</param>
	/// <returns></returns>
	bool AreFloatingPointsWithinTolerence(const float& A, const float& B, const float& tolerance);
	bool AreTwoPointsWithinTolerence(const Vector2& A, const Vector2& B, const float& tolerance);

	Vector2 TripleProduct(const Vector2& a, const Vector2& b, const Vector2& c);
};