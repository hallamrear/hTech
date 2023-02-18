#pragma once
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include <math.h>

class HTECH_FUNCTION_EXPORT Vector2
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

	void ToString(std::string& stringToFill);
	const std::string ToString() const;

	//Operators
	Vector2& operator= (const Vector2& other);
	Vector2 operator+(const Vector2& rhs);
	Vector2 operator-(const Vector2& rhs);
	Vector2 operator*(const float& scalar);
	Vector2 operator/(const float& scalar);
	Vector2& operator+=(const Vector2& rhs);
	Vector2& operator-=(const Vector2& rhs);
	Vector2& operator*=(const float& rhs);
	Vector2& operator*=(const Vector2& rhs);
	bool operator!=(const Vector2& rhs);
	bool operator==(const Vector2& rhs) const;
	const Vector2 operator-() const;

	const Vector2 operator+(const Vector2& rhs) const;
	const Vector2 operator-(const Vector2& rhs) const;
	const Vector2 operator*(const float& scalar) const;
	const Vector2 operator/(const float& scalar) const;

	void RotatePointAroundOriginRadians(float rotationRad, Vector2 origin);
	void RotatePointAroundOriginDegrees(float rotationDeg, Vector2 origin);
};

typedef Vector2 Point;