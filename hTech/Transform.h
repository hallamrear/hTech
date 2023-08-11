#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "Utility.h"

class HTECH_FUNCTION_EXPORT Transform
{
public:
	Vector2 Position;
	float Rotation;

	Transform(Vector2 position = Vector2(), float rotation = 0.0f);
	~Transform();

	void Reset();
	const Vector2 GetUp() const;
	const Vector2 GetDown() const;
	const Vector2 GetRight() const;
	const Vector2 GetLeft() const;
	void Translate(const Vector2 offset);
	void Rotate(const float angle);

	Transform operator+(const Transform& rhs) const
	{
		return Transform(Position + rhs.Position, Rotation + rhs.Rotation);;
	}
};