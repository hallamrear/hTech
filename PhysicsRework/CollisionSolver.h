#pragma once
#include "Vector2.h"
#include <vector>

class Body;
class Manifold;

struct ContactPoint
{
	Body*	BodyA;
	Body*	BodyB;
	Vector2 position = Vector2::Zero;
	Vector2 normal = Vector2::Zero;
	float   Depth = 0.0f;
	float   Bias = 0.2f;

	//Vector from BodyA->Position to position;
	Vector2 AC;
	//Vector from BodyB->Position to position;
	Vector2 BC;

	float MassAtNormal = 0.0f;
	float MassAtTangent = 0.0f;
	float ImpulseSum = 0.0f;
	float FrictionSum = 0.0f;
};

class CollisionSolver
{
	std::vector<ContactPoint> Points;

public:
	CollisionSolver(Manifold* manifold);

	void ConstuctContactPointsFromManifold(Manifold* manifold);

	void CalculateImpulses(const float& inverseDeltaTime);
	void ApplyImpulses();
};

