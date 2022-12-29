#include "MathsUtils.h"
#include "Collision.h"

//Works equally each direction from 0
//e.g. GetRandomIntExcludingCentre(256, 128)
//		can return between -256 -> -128 & 128 -> 256
int MathsUtils::GetRandomIntExcludingCentre(int upperBound, int lowerBound)
{
	int value = rand() % upperBound;

	if (value < lowerBound)
		value = (upperBound - value) * -1;

	return value;
}

float MathsUtils::ConvertToRadians(float degrees)
{
	return degrees * (float)(M_PI / 180.0);
}

float MathsUtils::ConvertToDegrees(float radians)
{
	return radians * (float)(180.0 / M_PI);
}

Vector2 MathsUtils::RotatePointAroundOriginRadians(Vector2 point, float rotation, Vector2 origin)
{
	float s = sin(rotation);
	float c = cos(rotation);

	// translate point back to origin:
	point.X -= origin.X;
	point.Y -= origin.Y;

	// rotate point
	float xnew = point.X * c - point.Y * s;
	float ynew = point.X * s + point.Y * c;

	// translate point back:
	Vector2 value;
	value.X = xnew + origin.X;
	value.Y = ynew + origin.Y;
	return value;
}

Vector2 MathsUtils::RotatePointAroundOriginDegrees(Vector2 point, float rotation, Vector2 origin)
{
	return RotatePointAroundOriginRadians(point, MathsUtils::ConvertToRadians(rotation), origin);
}

Vector2 MathsUtils::LerpPoint(Vector2 start, Vector2 end, float time)
{
	return (start * (1 - time) + end * time);
}

//Returns -1 if val < 0, 1 if val > 0, 0 if val == 0
int MathsUtils::Sign(int val)
{
	return ((0) < val) - (val < (0));
}

float MathsUtils::Dot(Vector2 a, Vector2 b)
{
	return (a.X * b.X) + (a.Y * b.Y);
}

float MathsUtils::GetAngleBetweenTwoVectorsDegrees(const Vector2& a, const Vector2& b)
{
	return ConvertToDegrees(GetAngleBetweenTwoVectors(a, b));
}

float MathsUtils::GetAngleBetweenTwoVectors(const Vector2& a, const Vector2& b)
{
	float dot = MathsUtils::Dot(a, b);
	float magnitudeSum = a.GetMagnitude() * b.GetMagnitude();
	float result = acos((dot / magnitudeSum));
	return result;
}

float MathsUtils::PointDistanceToLine(Point point, Line line)
{
	Vector2 AP = point - line.A;
	Vector2 AB = line.B - line.A;
	float angle = GetAngleBetweenTwoVectors(AB, AP);

	float distance = AP.GetMagnitude() * sin(angle);
	return distance;
}

float MathsUtils::PointDistanceToLineSigned(Point point, Line line)
{
	Vector2 AP = point - line.A;
	Vector2 AB = line.B - line.A;
	float angle = GetAngleBetweenTwoVectors(AB, AP);
	float distance = AP.GetMagnitude() * sin(angle);

	if (IsPointAboveLine(point, line) == false)
	{
		distance *= -1;
	}

	return distance;
}

bool MathsUtils::IsPointAboveLine(const Point& point, const Line& line)
{
	return ((line.B.X - line.A.X) * (point.Y - line.A.Y) - (line.B.Y - line.A.Y) * (point.X - line.A.X)) > 0;
}

Vector2 MathsUtils::CalculateIntersectionPointOfTwoLines(const Line& lineA, const Line& lineB)
{
	LineEquation equationA = lineA.GetEquation();
	LineEquation equationB = lineB.GetEquation();
	float intersectX = (equationB.C - equationA.C) / (equationA.M - equationB.M);
	float intersectY = equationA.M * intersectX + equationA.C;
	return Vector2(intersectX, intersectY);
}

MathsUtils::ClosestPointDistanceResult MathsUtils::FindClosestPointOnLine(const Vector2& point, const Vector2& A, const Vector2& B)
{
	MathsUtils::ClosestPointDistanceResult result;
	Vector2 AB = B - A;
	Vector2 AP = point - A;

	float projection = MathsUtils::Dot(AP, AB);
	float ABmagSquared = AB.GetMagnitudeSquared();
	float distance = projection / ABmagSquared;

	if (distance <= 0.0f)
	{
		result.ClosestPoint = A;
	}
	else if (distance >= 1.0f)
	{
		result.ClosestPoint = B;
	}
	else
	{
		result.ClosestPoint = A + AB * distance;
	}

	result.DistanceSquared = MathsUtils::DistanceSquared(point, result.ClosestPoint);

	return result;
}

MathsUtils::ClosestPointDistanceResult MathsUtils::FindClosestPointOnLine(const Vector2& point, const Line& AB)
{
	return MathsUtils::FindClosestPointOnLine(point, AB.A, AB.B);
}

bool MathsUtils::AreFloatingPointsWithinTolerence(const float& A, const float& B, const float& tolerence)
{
	return (abs(A - B) < tolerence);
}

bool MathsUtils::AreTwoPointsWithinTolerence(const Vector2& A, const Vector2& B, const float& tolerance)
{
	return (A - B).GetMagnitudeSquared() < (tolerance * tolerance);
}

float MathsUtils::DistanceSquared(const Vector2& a, const Vector2& b)
{
	float dx = a.X - b.X;
	float dy = a.Y - b.Y;
	return dx * dx + dy * dy;
};

float MathsUtils::Distance(const Vector2& a, const Vector2& b)
{
	float dx = a.X - b.X;
	float dy = a.Y - b.Y;
	return sqrtf(dx * dx + dy * dy);
};