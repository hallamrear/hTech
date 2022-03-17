#include "pch.h"
#include "Vector2.h"

Vector2f::Vector2f()
{
	X = 0.0f;
	Y = 0.0f;
}

Vector2f::Vector2f(float x, float y)
{
	X = x;
	Y = y;
}

//0deg == Vector(1.0, 0.0)
float Vector2f::GetAngleDegrees() const
{
	return HelperFunctions::ConvertToDegrees(GetAngleRadians());
}

//0r == Vector(1.0, 0.0)
float Vector2f::GetAngleRadians() const
{
	return atan2f(Y, X);
}

float Vector2f::GetMagnitude() const
{
	//Pythagoras
	return sqrtf((X * X) + (Y * Y));
}

float Vector2f::GetMagnitudeSquared() const
{
	return (X * X) + (Y * Y);
}

Vector2f Vector2f::GetNormalized() const
{
	float magnitude = GetMagnitude();
	return Vector2f(X / magnitude, Y / magnitude);
}

Vector2f Vector2f::GetVectorRotatedByAngle(float angleDegrees)
{
	float theta = HelperFunctions::ConvertToRadians(angleDegrees);
	float cs = cos(theta);
	float sn = sin(theta);

	Vector2f vec;
	vec.X = X * cs - Y * sn;
	vec.Y = X * sn + Y * cs;
	return vec;
}

//Operators
Vector2f& Vector2f::operator= (const Vector2f& other)
{
	this->X = other.X;
	this->Y = other.Y;

	return *this;
}

Vector2f Vector2f::operator+(const Vector2f& rhs)
{
	return Vector2f(this->X + rhs.X, this->Y + rhs.Y);
}

Vector2f Vector2f::operator-(const Vector2f& rhs)
{
	return Vector2f(this->X - rhs.X, this->Y - rhs.Y);
}

Vector2f Vector2f::operator*(const float& scalar)
{
	return Vector2f(this->X * scalar, this->Y * scalar);
}

Vector2f Vector2f::operator/(const float& scalar)
{
	return Vector2f(this->X / scalar, this->Y / scalar);
}

Vector2f& Vector2f::operator+=(const Vector2f& rhs)
{
	this->X += rhs.X;
	this->Y += rhs.Y;
	return *this;
}

Vector2f& Vector2f::operator-=(const Vector2f& rhs)
{
	this->X -= rhs.X;
	this->Y -= rhs.Y;
	return *this;
}

bool Vector2f::operator!=(const Vector2f& rhs)
{
	if ((this->X == rhs.X) && (this->Y == rhs.Y))
		return false;
	else
		return true;
}

float Vector2f::Dot(Vector2f vector) const
{
	return ((this->X * vector.X) + (this->Y * vector.Y));
}

bool Vector2f::operator==(const Vector2f& rhs)
{
	if ((this->X == rhs.X) && (this->Y == rhs.Y))
		return true;
	else
		return false;
}
//
//bool operator==(const Vector2f& lhs, const Vector2f& rhs)
//{
//	if ((lhs.X == rhs.X) && (lhs.Y == rhs.Y))
//		return true;
//	else
//		return false;
//}
//
//Vector2f operator-(const Vector2f& lhs, const Vector2f& rhs)
//{
//	return Vector2f(lhs.X - rhs.X, lhs.Y - rhs.Y);
//}