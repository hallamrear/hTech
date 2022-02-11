#pragma once
#include <math.h>

//todo : fix? vector2f for some reason does not understand it needs HTECH_API.
#ifdef HTECH_EXPORTS
#define HTECH_API __declspec(dllexport)
#else
#define HTECH_API __declspec(dllimport)
#endif

class HTECH_API Vector2f
{
public:
	float X;
	float Y;

	Vector2f();
	Vector2f(float x, float y);
	
	float GetAngleDegrees() const;
	float GetAngleRadians() const;
	float GetMagnitude() const;
	float GetMagnitudeSquared() const;
	Vector2f GetNormalized() const;
	Vector2f GetVectorRotatedByAngle(float angleDegrees);
	float Dot(Vector2f vector) const;

	//Operators
	Vector2f& operator= (const Vector2f& other);
	Vector2f operator+(const Vector2f& rhs);
	Vector2f operator-(const Vector2f& rhs);
	Vector2f operator*(const float& scalar);
	Vector2f operator/(const float& scalar);
	Vector2f& operator+=(const Vector2f& rhs);
	Vector2f& operator-=(const Vector2f& rhs);
	bool operator!=(const Vector2f& rhs);
	bool operator==(const Vector2f& rhs);
	//friend HTECH_API bool operator==(const Vector2f& lhs, const Vector2f& rhs);
	//friend HTECH_API Vector2f operator-(const Vector2f& lhs, const Vector2f& rhs);
};