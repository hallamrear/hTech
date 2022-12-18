#pragma once
#include "Vector2.h"
#include <SDL_rect.h>

struct Body
{
	Vector2 Pos;
	float	Rot;
	Vector2 Vel;
	float	AngularVel;
	Vector2 Force;
	float	Torque;
	float	Friction;

	float	Mass;
	float	InvMass;
	float	Inertia;
	float	InvInertia;

	Vector2 TL;
	Vector2 BL;
	Vector2 TR;
	Vector2 BR;
	int Size;

	Body(int x, int y, int size, float mass);
	~Body();

	void CalculateRotatedCorners();

	void Update(float dt);
};

