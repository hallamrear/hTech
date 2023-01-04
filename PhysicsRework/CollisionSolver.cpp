#include "CollisionSolver.h"
#include "Body.h"
#include "MathsUtils.h"
#include "Collision.h"

using namespace MathsUtils;

CollisionSolver::CollisionSolver(const Manifold& manifold)
{
	ConstuctContactPointsFromManifold(manifold);
}

CollisionSolver::~CollisionSolver()
{
	m_ContactPoints.clear();
}

void CollisionSolver::ConstuctContactPointsFromManifold(const Manifold& manifold)
{
	for (size_t i = 0; i < manifold.ContactPoints.size(); i++)
	{
		ContactPoint point;
		point.BodyA = manifold.BodyA;
		point.BodyB = manifold.BodyB;
		point.Depth = manifold.Depth;
		point.Position = manifold.ContactPoints[i];
		point.Normal = manifold.Normal;
		point.Depth = manifold.Depth;
		m_ContactPoints.push_back(point);
	}
}

void GJKCollisionSolver::Prestep(const float& inverseDeltaTime)
{

}

void GJKCollisionSolver::PhysicsStep()
{

}