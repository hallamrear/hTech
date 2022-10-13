#include "pch.h"
#include "Transform.h"

Transform::Transform(Vector2f position, float rotation)
{
	this->Position = position;
	this->Rotation = rotation;
}

Transform::~Transform()
{
	Reset();
}

void Transform::Reset()
{
	Position = Vector2f(0.0f, 0.0f);
	Rotation = 0.0f;
}

Vector2f Transform::GetUp() const
{
	return Vector2f(HelperFunctions::RotatePointAroundOriginDegrees(Vector2f(0.0f, 1.0f), 360.0f - Rotation, Vector2f())).GetNormalized();
}

Vector2f Transform::GetDown() const
{
	return Vector2f(HelperFunctions::RotatePointAroundOriginDegrees(Vector2f(0.0f, -1.0f), 360.0f - Rotation, Vector2f()).GetNormalized());
}

Vector2f Transform::GetRight() const
{
	return Vector2f(HelperFunctions::RotatePointAroundOriginDegrees(Vector2f(1.0f, 0.0f), 360.0f - Rotation, Vector2f()).GetNormalized());
}

Vector2f Transform::GetLeft() const
{
	return Vector2f(HelperFunctions::RotatePointAroundOriginDegrees(Vector2f(-1.0f, 0.0f), 360.0f - Rotation, Vector2f()).GetNormalized());
}

void Transform::AdjustPosition(const Vector2f offset)
{
	Position += offset;
}

void Transform::AdjustRotation(const float angle)
{
	Rotation += angle;
}