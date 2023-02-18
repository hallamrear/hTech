#include "pch.h"
#include "Vector2.h"

Vector2::Vector2()
{
	X = 0.0f;
	Y = 0.0f;
}

Vector2::Vector2(float x, float y)
{
	X = x;
	Y = y;
}

//0deg == Vector(1.0, 0.0)
float Vector2::GetAngleDegrees() const
{
	return Utils::Maths::ConvertToDegrees(GetAngleRadians());
}

//0r == Vector(1.0, 0.0)
float Vector2::GetAngleRadians() const
{
	return atan2f(Y, X);
}

float Vector2::GetMagnitude() const
{
	//Pythagoras
	return sqrtf((X * X) + (Y * Y));
}

float Vector2::GetMagnitudeSquared() const
{
	return (X * X) + (Y * Y);
}

Vector2 Vector2::GetNormalized() const
{
	float magnitude = GetMagnitude();
	return Vector2(X / magnitude, Y / magnitude);
}

Vector2 Vector2::GetVectorRotatedByAngle(float angleDegrees)
{
	float theta = Utils::Maths::ConvertToRadians(angleDegrees);
	float cs = cos(theta);
	float sn = sin(theta);

	Vector2 vec;
	vec.X = X * cs - Y * sn;
	vec.Y = X * sn + Y * cs;
	return vec;
}

//Operators
Vector2& Vector2::operator= (const Vector2& other)
{
	this->X = other.X;
	this->Y = other.Y;

	return *this;
}

Vector2 Vector2::operator+(const Vector2& rhs)
{
	return Vector2(this->X + rhs.X, this->Y + rhs.Y);
}

Vector2 Vector2::operator-(const Vector2& rhs)
{
	return Vector2(this->X - rhs.X, this->Y - rhs.Y);
}

Vector2 Vector2::operator*(const float& scalar)
{
	return Vector2(this->X * scalar, this->Y * scalar);
}

Vector2 Vector2::operator/(const float& scalar)
{
	return Vector2(this->X / scalar, this->Y / scalar);
}

Vector2& Vector2::operator+=(const Vector2& rhs)
{
	this->X += rhs.X;
	this->Y += rhs.Y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& rhs)
{
	this->X -= rhs.X;
	this->Y -= rhs.Y;
	return *this;
}

bool Vector2::operator!=(const Vector2& rhs)
{
	if ((this->X == rhs.X) && (this->Y == rhs.Y))
		return false;
	else
		return true;
}

float Vector2::Dot(Vector2 vector) const
{
	return ((this->X * vector.X) + (this->Y * vector.Y));
}

//Adds the vector as a string to the end of 'stringToFill'.
//i.e. str += "X, Y";
void Vector2::ToString(std::string& stringToFill)
{
	stringToFill += X;
	stringToFill += ", ";
	stringToFill += Y;
}

//Returns a vector as a string.
//i.e. std::string("X, Y");
const std::string Vector2::ToString() const
{
	return std::string(std::to_string(X) + ", " + std::to_string(Y));
}

bool Vector2::operator==(const Vector2& rhs) const
{
	return ((this->X == rhs.X) && (this->Y == rhs.Y));
}

void Vector2::RotatePointAroundOriginRadians(float rotationRad, Vector2 origin)
{
	float s = sin(rotationRad);
	float c = cos(rotationRad);

	// translate point back to origin:
	X -= origin.X;
	Y -= origin.Y;

	// rotate point
	float xnew = X * c - Y * s;
	float ynew = X * s + Y * c;

	// translate point back:
	X = xnew + origin.X;
	Y = ynew + origin.Y;
}

void Vector2::RotatePointAroundOriginDegrees(float rotationDeg, Vector2 origin)
{
	float rotationRad = Utils::Maths::ConvertToRadians(rotationDeg);
	float s = sin(rotationRad);
	float c = cos(rotationRad);

	// translate point back to origin:
	X -= origin.X;
	Y -= origin.Y;

	// rotate point
	float xnew = X * c - Y * s;
	float ynew = X * s + Y * c;

	// translate point back:
	X = xnew + origin.X;
	Y = ynew + origin.Y;
}


const Vector2 Vector2::operator+(const Vector2& rhs) const
{
	return Vector2(this->X + rhs.X, this->Y + rhs.Y);
}

const Vector2 Vector2::operator-(const Vector2& rhs) const
{
	return Vector2(this->X - rhs.X, this->Y - rhs.Y);
}

const Vector2 Vector2::operator*(const float& scalar) const
{
	return Vector2(this->X * scalar, this->Y * scalar);
}

const Vector2 Vector2::operator/(const float& scalar) const
{
	return Vector2(this->X / scalar, this->Y / scalar);
}

const Vector2 Vector2::operator-() const
{
	return Vector2(-X, -Y);
}

Vector2& Vector2::operator*=(const float& rhs)
{
	this->X *= rhs;
	this->Y *= rhs;
	return *this;
}

Vector2& Vector2::operator*=(const Vector2& rhs)
{
	this->X *= rhs.X;
	this->Y *= rhs.Y;
	return *this;
}