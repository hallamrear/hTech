#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)


#include "Helper.h"

class HTECH_FUNCTION_EXPORT Transform
{
public:
	Vector2 Position;
	float Rotation;

	Transform(Vector2 position = Vector2(), float rotation = 0.0f);
	~Transform();

	void Reset();
	Vector2 GetUp() const;
	Vector2 GetDown() const;
	Vector2 GetRight() const;
	Vector2 GetLeft() const;
	void Translate(const Vector2 offset);
	void Rotate(const float angle);
};

