#include "Vector3.h"
#include <math.h>

Vector3::Vector3(float x, float y, float z)
{
	X = x;
	Y = y;
	Z = z;
}

Vector3::~Vector3()
{

}

float Vector3::Dot(const Vector3& other)
{
    float angle = (X * other.X) + (Y * other.Y) + (Z * other.Z);
    return angle;
}

//Normalises this vector and stores it. Use Normalised() for a normalised version of this vector.
void Vector3::Normalise()
{
    float magnitude = GetMagnitude();
    this->X /= magnitude;
    this->Y /= magnitude;
    this->Z /= magnitude;
}

//Returns a normalised version of this vector. Use Normalise() to set this vector to its normalised form.
Vector3 Vector3::GetNormalized() const
{
    float magnitude = GetMagnitude();
    Vector3 normalised;
    normalised.X = X / magnitude;
    normalised.Y = Y / magnitude;
    normalised.Z = Z / magnitude;
    return normalised;
}

Vector3 Vector3::Cross(const Vector3& A, const Vector3& B)
{
    Vector3 ans;
    ans.X = (A.Y * B.Z) - (A.Z * B.Y);
    ans.Y = (A.Z * B.X) - (A.X * B.Z);
    ans.Z = (A.X * B.Y) - (A.Y * B.X);
    return ans;
}

Vector3 Vector3::Cross(const Vector3& other)
{
    Vector3 ans;
    ans.X = (Y * other.Z) - (Z * other.Y);
    ans.Y = (Z * other.X) - (X * other.Z);
    ans.Z = (X * other.Y) - (Y * other.X);
    return ans;
}

float Vector3::GetMagnitude() const
{
    return sqrtf((X * X) + (Y * Y) + (Z * Z));
}

float Vector3::GetMagnitudeSquared() const
{
    return ((X * X) + (Y * Y) + (Z * Z));
}