#pragma once
#include "Helper.h"

class Transform
{
public:
	Vector2f Position;
	float Rotation;

	Transform(Vector2f position = Vector2f(), float rotation = 0.0f);
	~Transform();
	void Reset();
	Vector2f GetUp() const;
	Vector2f GetDown() const;
	Vector2f GetRight() const;
	Vector2f GetLeft() const;
	void AdjustPosition(const Vector2f offset);
	void AdjustRotation(const float angle);
};

