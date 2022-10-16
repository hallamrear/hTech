#pragma once
#include "Helper.h"

class Transform
{
public:
	//Todo : add scaling;
	//float Scale;

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

