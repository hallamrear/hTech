#include "pch.h"
#include "Transform.h"

Transform::Transform(Vector2 position, float rotation)
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
	Position = Vector2(0.0f, 0.0f);
	Rotation = 0.0f;
}

const Vector2 Transform::GetUp() const
{
	return Vector2(MathsHelp::RotatePointAroundOriginDegrees(Vector2(0.0f, 1.0f), 360.0f - Rotation, Vector2())).GetNormalized();
}

const Vector2 Transform::GetDown() const
{
	return Vector2(MathsHelp::RotatePointAroundOriginDegrees(Vector2(0.0f, -1.0f), 360.0f - Rotation, Vector2()).GetNormalized());
}

const Vector2 Transform::GetRight() const
{
	return Vector2(MathsHelp::RotatePointAroundOriginDegrees(Vector2(1.0f, 0.0f), 360.0f - Rotation, Vector2()).GetNormalized());
}

const Vector2 Transform::GetLeft() const
{
	return Vector2(MathsHelp::RotatePointAroundOriginDegrees(Vector2(-1.0f, 0.0f), 360.0f - Rotation, Vector2()).GetNormalized());
}

void Transform::Translate(const Vector2 offset)
{
	Position += offset;
}

void Transform::Rotate(const float angle)
{
	Rotation += angle;
}