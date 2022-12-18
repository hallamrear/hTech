#include "Body.h"
#include "MathsUtils.h"

Body::Body(int x, int y, int size, float mass)
{
	srand(NULL);
	Size = size;
	Pos.X = x;
	Pos.Y = y;

	Rot = 0.0f;
	Vel = Vector2::Zero;
	AngularVel = 0.0f;
	Force = Vector2::Zero;
	Torque = 0.0f;
	Friction = 0.0f;

	if (mass < FLT_MAX)
	{
		InvMass = 1.0f / Mass;
		Inertia = Mass * (Size * Size + Size * Size) / 12.0f;
		InvInertia = 1.0f / Inertia;
	}
	else
	{
		InvMass = 0.0f;
		Inertia = FLT_MAX;
		InvMass = 0.0f;
	}

	CalculateRotatedCorners();
}

Body::~Body()
{
}

void Body::CalculateRotatedCorners()
{
	//Calculate new point rotations;
	if (Rot != 0.0f || Rot != 360.0f)
	{
		float rotation = 360.0f - Rot;
		TL = MathsUtils::RotatePointAroundOriginDegrees(Vector2(Pos.X - (Size / 2), Pos.Y + (Size / 2)), rotation, Pos);
		BL = MathsUtils::RotatePointAroundOriginDegrees(Vector2(Pos.X - (Size / 2), Pos.Y - (Size / 2)), rotation, Pos);
		TR = MathsUtils::RotatePointAroundOriginDegrees(Vector2(Pos.X + (Size / 2), Pos.Y + (Size / 2)), rotation, Pos);
		BR = MathsUtils::RotatePointAroundOriginDegrees(Vector2(Pos.X + (Size / 2), Pos.Y - (Size / 2)), rotation, Pos);
	}
}

void Body::Update(float dt)
{
	CalculateRotatedCorners();
}
