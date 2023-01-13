#include "CollisionSolver.h"
#include "Body.h"
#include "MathsUtils.h"
#include "Collision.h"
#include "Body.h"

using namespace MathsUtils;

CollisionSolver::CollisionSolver(const Manifold& manifold)
{
	ConstuctContactPointsFromManifold(manifold);
	if (m_ContactPoints.size() == 1)
		m_PointSplit = 1.0f;
	else
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
		if (m_ContactPoints[i].BodyA->IsStatic() == true && m_ContactPoints[i].BodyB->IsStatic() == true)
			continue;

		if (m_ContactPoints[i].BodyA->IsStatic() == false && m_ContactPoints[i].BodyB->IsStatic() == true)
		{
			m_ContactPoints[i].BodyA->Pos += m_ContactPoints[i].Normal * (-m_ContactPoints[i].Depth * m_PointSplit);
		}
		else if (m_ContactPoints[i].BodyA->IsStatic() == true && m_ContactPoints[i].BodyB->IsStatic() == false)
		{
			m_ContactPoints[i].BodyB->Pos += m_ContactPoints[i].Normal * (m_ContactPoints[i].Depth * m_PointSplit);
		}
		else
		{
			m_ContactPoints[i].BodyA->Pos += m_ContactPoints[i].Normal * (m_ContactPoints[i].Depth * -0.5f * m_PointSplit);
			m_ContactPoints[i].BodyB->Pos += m_ContactPoints[i].Normal * (m_ContactPoints[i].Depth * 0.5f * m_PointSplit);
		}
	}
}

void CollisionSolver::ConstuctContactPointsFromManifold(const Manifold& manifold)
{
	//for (size_t i = 0; i < manifold.ContactPoints.size(); i++)
	//{
	//	ContactPoint point;
	//	point.BodyA = manifold.BodyA;
	//	point.BodyB = manifold.BodyB;
	//	point.Depth = manifold.Depth;
	//	point.Position = manifold.ContactPoints[i];
	//	point.Normal = manifold.Normal;
	//	point.Depth = manifold.Depth;
	//	m_ContactPoints.push_back(point);
	//}

	ContactPoint point;
	point.BodyA = manifold.BodyA;
	point.BodyB = manifold.BodyB;
	point.Depth = manifold.Depth;
	point.Normal = manifold.Normal;
	m_ContactPoints.push_back(point);
}

void GJKCollisionSolver::Prestep()
{

}

void GJKCollisionSolver::PhysicsStep()
{
	for (size_t i = 0; i < m_ContactPoints.size(); i++)
	{
		//Get our current contact point.
		ContactPoint& point = m_ContactPoints[i];

		Body* bodyA = nullptr;
		if (point.BodyA != nullptr)
		{
			bodyA = point.BodyA;
		}

		Body* bodyB = nullptr;
		if (point.BodyB != nullptr)
		{
			bodyB = point.BodyB;
		}

		if (bodyA == nullptr && bodyB == nullptr)
		{
			return;
		}
		
		Vector2 bodyAVelocity = Vector2();
		if (bodyA) bodyAVelocity = bodyA->Vel;

		Vector2 bodyBVelocity = Vector2();
		if (bodyB) bodyBVelocity = bodyB->Vel;
		
		Vector2 relativeVelocity = bodyBVelocity - bodyAVelocity;
		float normalVelocity = Dot(relativeVelocity, point.Normal);

		float bodyAInvMass = 1.0f;
		if (bodyA) bodyAInvMass = bodyA->InvMass;

		float bodyBInvMass = 1.0f;
		if (bodyB) bodyBInvMass = bodyB->InvMass;

		// Impluse
		if (normalVelocity >= 0)
			return;

		float restA = 1.0f;
		if (bodyA) restA = bodyA->Material.Restitution;

		float restB = 1.0f;
		if (bodyB) restB = bodyB->Material.Restitution;

		float e = restA * restB;

		float j = -(1.0f + e) * normalVelocity / (bodyAInvMass + bodyBInvMass);

		Vector2 impluse = point.Normal * j;

		if (bodyA != nullptr)
		{
			if (!bodyA->IsStatic())
			{
				bodyAVelocity -= impluse * bodyAInvMass * m_PointSplit;
			}
		}

		if (bodyB != nullptr)
		{
			if (!bodyB->IsStatic())
			{
				bodyBVelocity += impluse * bodyBInvMass * m_PointSplit;
			}
		}

		// Friction

		relativeVelocity = bodyBVelocity - bodyAVelocity;
		normalVelocity = Dot(relativeVelocity, point.Normal);

		Vector2 tangent = relativeVelocity - point.Normal * normalVelocity;

		if(tangent.GetMagnitude() > 0.00001f)
			tangent = tangent.GetNormalized();

		float frictionVelocity = Dot(relativeVelocity, tangent);

		float bodyAStaticFriction = 0.0f;
		if (bodyA) bodyAStaticFriction = bodyA->Material.StaticFriction;

		float bodyBStaticFriction = 0.0f;
		if (bodyB) bodyBStaticFriction = bodyB->Material.StaticFriction;

		float bodyADynamicFriction = 0.0f;
		if(bodyA) bodyADynamicFriction = bodyA->Material.DynamicFriction;

		float bodyBDynamicFriction = 0.0f;
		if (bodyB) bodyBDynamicFriction = bodyB->Material.DynamicFriction;
		
		float mu = Vector2(bodyAStaticFriction, bodyBStaticFriction).GetMagnitude();
		float f = -frictionVelocity / (bodyAInvMass + bodyBInvMass);

		Vector2 friction = Vector2();
		if (abs(f) < j * mu)
		{
			friction = tangent * f;
		}
		else 
		{
			mu = Vector2(bodyADynamicFriction, bodyBDynamicFriction).GetMagnitude();
			friction = tangent * mu * -j;
		}

		if (bodyA != nullptr)
		{
			if (!bodyA->IsStatic())
			{
				bodyA->Vel = bodyAVelocity - friction * bodyAInvMass * m_PointSplit;
			}
		}
		
		if (bodyB != nullptr)
		{
			if (!bodyB->IsStatic())
			{
				bodyB->Vel = bodyBVelocity + friction * bodyBInvMass * m_PointSplit;
			}
		}
	}
}