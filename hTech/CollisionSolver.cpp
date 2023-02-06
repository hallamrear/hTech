#include "pch.h"
#include "CollisionSolver.h"
#include "CollisionManifold.h"
#include "Component_Rigidbody.h"
#include "Utility.h"
#include "Entity.h"

using namespace Utils::Maths;

CollisionSolver::CollisionSolver(const CollisionManifold& manifold)
{
	m_PointSplit = 1.0f;

	ConstuctContactPointsFromManifold(manifold);

	if (m_ContactPoints.size() > 1)
		m_PointSplit = 1.0f / m_ContactPoints.size();
}

CollisionSolver::~CollisionSolver()
{
	m_ContactPoints.clear();
}

void CollisionSolver::PositionResolution()
{

	for (size_t i = 0; i < m_ContactPoints.size(); i++)
	{
		Transform& transformA = m_ContactPoints[i].BodyA->GetEntity().GetTransform();
		Transform& transformB = m_ContactPoints[i].BodyB->GetEntity().GetTransform();

		if (m_ContactPoints[i].BodyA->GetIsStatic() == true && m_ContactPoints[i].BodyB->GetIsStatic() == true)
			continue;

		if (m_ContactPoints[i].BodyA->GetIsStatic() == false && m_ContactPoints[i].BodyB->GetIsStatic() == true)
		{
			transformA.Position += m_ContactPoints[i].Normal * (-m_ContactPoints[i].Depth * m_PointSplit);
		}
		else if (m_ContactPoints[i].BodyA->GetIsStatic() == true && m_ContactPoints[i].BodyB->GetIsStatic() == false)
		{
			transformB.Position += m_ContactPoints[i].Normal * (m_ContactPoints[i].Depth * m_PointSplit);
		}
		else
		{
			transformA.Position += m_ContactPoints[i].Normal * (m_ContactPoints[i].Depth * -0.5f * m_PointSplit);
			transformB.Position += m_ContactPoints[i].Normal * (m_ContactPoints[i].Depth * 0.5f * m_PointSplit);
		}
	}
}

void CollisionSolver::ConstuctContactPointsFromManifold(const CollisionManifold& manifold)
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