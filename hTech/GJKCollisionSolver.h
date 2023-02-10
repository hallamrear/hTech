#pragma once
#include "CollisionSolver.h"
#include "CollisionManifold.h"

/// <summary>
/// Implementation of Collision solver specifically to work with the GJK collision detection.
/// </summary>
class GJKCollisionSolver : public CollisionSolver
{
protected:

public:
	GJKCollisionSolver(const CollisionManifold& manifold);
	~GJKCollisionSolver();

	void Prestep();
	void PhysicsStep();
};