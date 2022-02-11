#pragma once
#include "Helper.h"

class HTECH_API Transform
{
public:
	Vector2f Position;
	float Rotation;

	Transform();
	Transform(Vector2f position, float rotation);
	~Transform();
	void Reset();
	Vector2f GetUp() const;
	Vector2f GetDown() const;
	Vector2f GetRight() const;
	Vector2f GetLeft() const;
	void AdjustPosition(const Vector2f offset);
	void AdjustRotation(const float angle);
};

