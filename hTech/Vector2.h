#pragma once
#include <math.h>

class Vector2
{
public:
	float X;
	float Y;

	Vector2();
	Vector2(float x, float y);
	
	float GetAngleDegrees() const;
	float GetAngleRadians() const;
	float GetMagnitude() const;
	float GetMagnitudeSquared() const;
	Vector2 GetNormalized() const;
	Vector2 GetVectorRotatedByAngle(float angleDegrees);
	float Dot(Vector2 vector) const;

	//Operators
	Vector2& operator= (const Vector2& other);
	Vector2 operator+(const Vector2& rhs);
	Vector2 operator-(const Vector2& rhs);
	Vector2 operator*(const float& scalar);
	Vector2 operator/(const float& scalar);
	Vector2& operator+=(const Vector2& rhs);
	Vector2& operator-=(const Vector2& rhs);
	bool operator!=(const Vector2& rhs);
	bool operator==(const Vector2& rhs);

	void RotatePointAroundOriginRadians(float rotationRad, Vector2 origin);
	void RotatePointAroundOriginDegrees(float rotationDeg, Vector2 origin);
	//friend bool operator==(const Vector2& lhs, const Vector2& rhs);
	//friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
};