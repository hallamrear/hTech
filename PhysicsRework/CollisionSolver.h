#pragma once
#include "Vector2.h"
#include "Manifold.h"
#include <vector>

struct Body;

class CollisionSolver
{
protected:
	struct ContactPoint
	{
		Body* BodyA;
		Body* BodyB;
		Vector2 Position = Vector2::Zero;
		Vector2 Normal = Vector2::Zero;
		float   Depth = 0.0f;
		float   Bias = 0.2f;

		//Vector from BodyA->Position to Position;
		Vector2 AC;
		//Vector from BodyB->Position to Position;
		Vector2 BC;

		float MassAtNormal = 0.0f;
		float MassAtTangent = 0.0f;
		float ImpulseSum = 0.0f;
		float FrictionSum = 0.0f;
	};

	std::vector<ContactPoint> m_ContactPoints;

	void ConstuctContactPointsFromManifold(const Manifold& manifold);

public:
	CollisionSolver(const Manifold& manifold);
	virtual ~CollisionSolver() = 0;
	virtual void CalculateImpulses(const float& inverseDeltaTime) = 0;
	virtual void ApplyImpulses() = 0;
};

class DynamicVsDynamicCollisionSolver : public CollisionSolver
{
public:
	DynamicVsDynamicCollisionSolver(const Manifold& manifold) : CollisionSolver(manifold) {};
	~DynamicVsDynamicCollisionSolver() {};

	void CalculateImpulses(const float& inverseDeltaTime);
	void ApplyImpulses();
};

class StaticVsDynamicCollisionSolver : public CollisionSolver
{
public:	
	StaticVsDynamicCollisionSolver(const Manifold& manifold) : CollisionSolver(manifold) {};
	~StaticVsDynamicCollisionSolver() {};

	void CalculateImpulses(const float& inverseDeltaTime);
	void ApplyImpulses();
};