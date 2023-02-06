#include "pch.h"
#include "GJKCollisionSolver.h"
#include "Component_Rigidbody.h"
#include "Utility.h"

using namespace Utils::Maths;

GJKCollisionSolver::GJKCollisionSolver(const CollisionManifold& manifold) : CollisionSolver(manifold)
{

}

GJKCollisionSolver::~GJKCollisionSolver()
{
	m_ContactPoints.clear();
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

		RigidbodyComponent* bodyA = nullptr;
		if (point.BodyA != nullptr)
		{
			bodyA = point.BodyA;
		}

		RigidbodyComponent* bodyB = nullptr;
		if (point.BodyB != nullptr)
		{
			bodyB = point.BodyB;
		}

		if (bodyA == nullptr && bodyB == nullptr)
		{
			return;
		}

		Vector2 bodyAVelocity = Vector2();
		if (bodyA) bodyAVelocity = bodyA->GetVelocity();

		Vector2 bodyBVelocity = Vector2();
		if (bodyB) bodyBVelocity = bodyB->GetVelocity();

		Vector2 relativeVelocity = bodyBVelocity - bodyAVelocity;
		float normalVelocity = Dot(relativeVelocity, point.Normal);

		float bodyAInvMass = 1.0f;
		if (bodyA) bodyAInvMass = bodyA->GetInverseMass();

		float bodyBInvMass = 1.0f;
		if (bodyB) bodyBInvMass = bodyB->GetInverseMass();

		// Impluse
		if (normalVelocity >= 0)
			return;

		float restA = 1.0f;
		if (bodyA) restA = bodyA->GetRestitution();

		float restB = 1.0f;
		if (bodyB) restB = bodyB->GetRestitution();

		float e = restA * restB;

		if (bodyAInvMass + bodyBInvMass == 0.0f)
		{
			return;
		}

		float j = -(1.0f + e) * normalVelocity / (bodyAInvMass + bodyBInvMass);

		Vector2 impluse = point.Normal * j;

		if (bodyA != nullptr)
		{
			if (!bodyA->GetIsStatic())
			{
				bodyAVelocity -= impluse * bodyAInvMass * m_PointSplit;
			}
		}

		if (bodyB != nullptr)
		{
			if (!bodyB->GetIsStatic())
			{
				bodyBVelocity += impluse * bodyBInvMass * m_PointSplit;
			}
		}

		// Friction

		relativeVelocity = bodyBVelocity - bodyAVelocity;
		normalVelocity = Dot(relativeVelocity, point.Normal);

		Vector2 tangent = relativeVelocity - point.Normal * normalVelocity;

		if (tangent.GetMagnitude() > 0.00001f)
			tangent = tangent.GetNormalized();

		float frictionVelocity = Dot(relativeVelocity, tangent);

		float bodyAStaticFriction = 0.0f;
		if (bodyA) bodyAStaticFriction = bodyA->GetStaticFriction();

		float bodyBStaticFriction = 0.0f;
		if (bodyB) bodyBStaticFriction = bodyB->GetStaticFriction();

		float bodyADynamicFriction = 0.0f;
		if (bodyA) bodyADynamicFriction = bodyA->GetDynamicFriction();

		float bodyBDynamicFriction = 0.0f;
		if (bodyB) bodyBDynamicFriction = bodyB->GetDynamicFriction();

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
			if (!bodyA->GetIsStatic())
			{
				bodyA->SetVelocity(bodyAVelocity - friction * -bodyAInvMass * m_PointSplit);
			}
		}

		if (bodyB != nullptr)
		{
			if (!bodyB->GetIsStatic())
			{
				bodyB->SetVelocity(bodyBVelocity + friction * -bodyBInvMass * m_PointSplit);
			}
		}
	}

	PositionResolution();
}