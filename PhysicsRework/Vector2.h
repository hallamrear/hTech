#pragma once
#include <math.h>
#include <string>

#include "SDL.h"

class Vector2
{
public:

	float X;
	float Y;

	Vector2();
	Vector2(float X, float Y);

	float GetAngleDegrees() const;
	float GetAngleRadians() const;
	float GetMagnitude() const;
	float GetMagnitudeSquared() const;
	Vector2 GetNormalized() const;
	Vector2 GetVectorRotatedByAngle(float angleDegrees);
	float Dot(Vector2 vector) const;

	void ToString(std::string& stringToFill);
	const std::string ToString() const;

	//Operators
	Vector2 operator-();
	Vector2& operator= (const Vector2& other);
	Vector2 operator+(const Vector2& rhs);
	Vector2 operator-(const Vector2& rhs);
	Vector2 operator*(const float& scalar);
	Vector2 operator/(const float& scalar);
	Vector2& operator*=(const float& rhs);
	Vector2& operator*=(const Vector2& rhs);
	Vector2& operator+=(const Vector2& rhs);
	Vector2& operator-=(const Vector2& rhs);
	bool operator!=(const Vector2& rhs);
	bool operator==(const Vector2& rhs) const;

	const Vector2 operator+(const Vector2& rhs) const;
	const Vector2 operator-(const Vector2& rhs) const;
	const Vector2 operator*(const float& scalar) const;
	const Vector2 operator/(const float& scalar) const;

	void RotatePointAroundOriginRadians(float rotationRad, Vector2 origin);
	void RotatePointAroundOriginDegrees(float rotationDeg, Vector2 origin);

	static const Vector2 Zero;
	static const Vector2 Infinity;
};

typedef Vector2 Point;
class Edge;

struct LineEquation
{
	float M = 0;
	float C = 0;

	LineEquation(float m, float c) : M(m), C(c) {};
};

struct Line
{
	SDL_Color colour;

	Point A;
	Point B;

	Line() {};
	Line(const Point& a, const Point& b) : A(a), B(b)
	{ 
		colour.r = 255;
		colour.g = 255;
		colour.b = 0;
		colour.a = 255;
	};
	Line(const Point& a, const Point& b, SDL_Color color) : A(a), B(b), colour(color) {};

	Line& operator=(const Line& edge)
	{
		A = edge.A;
		B = edge.B;
		return *this;
	};

	Vector2 GetNormal() const
	{
		float dx = B.X - A.X;
		float dy = B.Y - A.Y;
		return Vector2(-dy, dx).GetNormalized();
	}

	Vector2 GetCentrePoint() const
	{
		return Vector2((A.X + B.X) / 2.0f, (A.Y + B.Y) / 2.0f);
	}

	LineEquation GetEquation() const
	{
		float dY = (B.Y - A.Y);
		float dX = (B.X - A.X);
		float m = 0.0f;

		if (dY != 0.0f && dX != 0.0f)
		{
			m = (B.Y - A.Y) / (B.X - A.X);
		}

		float c = A.Y - (m * A.X);
		return LineEquation(m, c);
	}
};