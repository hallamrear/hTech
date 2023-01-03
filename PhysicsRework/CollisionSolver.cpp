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
		point.Position  = manifold.ContactPoints[i];
		point.Normal = manifold.Normal;
		point.Depth = manifold.Depth;
		m_ContactPoints.push_back(point);
	}
}

void DynamicVsDynamicCollisionSolver::Prestep(const float& inverseDeltaTime)
{
	size_t count = m_ContactPoints.size();
	float positionCorrection = 0.2f;

	for (size_t i = 0; i < count; i++)
	{
		ContactPoint& contact = m_ContactPoints[i];

		contact.AC = contact.Position - contact.BodyA->Pos;
		contact.BC = contact.Position - contact.BodyB->Pos;

		//Compute contact point Normal data
		float ACNormal = Dot(contact.AC, contact.Normal);
		float BCNormal = Dot(contact.BC, contact.Normal);

		float inverseMassSum = contact.BodyA->InvMass + contact.BodyB->InvMass;

		float massAtNormal = inverseMassSum;
		massAtNormal += contact.BodyA->InvInertia * (Dot(contact.AC, contact.AC) - ACNormal * ACNormal);
		massAtNormal += contact.BodyB->InvInertia * (Dot(contact.BC, contact.BC) - BCNormal * BCNormal);

		contact.MassAtNormal = 1.0f / massAtNormal;

		//Compute contact point tangential data
		Vector2 tangent = Cross(contact.Normal, 1.0f);
		float ACTangent = Dot(contact.AC, tangent);
		float BCTangent = Dot(contact.BC, tangent);

		float massAtTangent = inverseMassSum;
		massAtTangent += contact.BodyA->InvInertia * (Dot(contact.AC, contact.AC) - ACTangent * ACTangent);
		massAtTangent += contact.BodyB->InvInertia * (Dot(contact.BC, contact.BC) - BCTangent * BCTangent);
		contact.MassAtTangent = 1.0f / massAtTangent;

		contact.Bias = (-positionCorrection) * inverseDeltaTime * (std::min(0.0f, -contact.Depth + COLLISION_SKIN_DISTANCE));

		//Apply Normal + frictional contactImpulse to contact point.
		//Applying anti-pen contactImpulse
		Vector2 impulse = (contact.Normal * contact.ImpulseSum);
		//Applying frictional contactImpulse
		impulse += (tangent * contact.FrictionSum);

		contact.BodyA->Vel -= (impulse * contact.BodyA->InvMass);
		contact.BodyA->AngularVel -= (contact.BodyA->InvInertia * CrossMagnitude(contact.AC, impulse));

		contact.BodyB->Vel += (impulse * contact.BodyB->InvMass);
		contact.BodyB->AngularVel -= (contact.BodyB->InvInertia * CrossMagnitude(contact.BC, impulse));
	}
}

void DynamicVsDynamicCollisionSolver::PhysicsStep()
{
	size_t count = m_ContactPoints.size();

	for (size_t i = 0; i < count; i++)
	{
		ContactPoint& contact = m_ContactPoints[i];

		//Relative velocity at contact.
		Vector2 relativeVelocity = 
			contact.BodyB->Vel + Cross(contact.BodyB->AngularVel, contact.BC) 
		  - contact.BodyA->Vel - Cross(contact.BodyA->AngularVel, contact.AC);


		//Calculate Normal impulse
		float normalImpulse = Dot(relativeVelocity, contact.Normal);

		float dPn = contact.MassAtNormal * (-normalImpulse + contact.Bias);

		//Clamp accumulated impulse
		float clampedImpulse = contact.ImpulseSum;
		contact.ImpulseSum = std::max(clampedImpulse + dPn, 0.0f);
		dPn = contact.ImpulseSum - clampedImpulse;

		//Apply contact impulse
		Vector2 contactNormalImpulse = contact.Normal * dPn;
		contact.BodyA->Vel -= contactNormalImpulse * contact.BodyA->InvMass;
		contact.BodyA->AngularVel -= CrossMagnitude(contact.AC, contactNormalImpulse) * contact.BodyA->InvInertia;
		contact.BodyB->Vel += contactNormalImpulse * contact.BodyB->InvMass;
		contact.BodyB->AngularVel += CrossMagnitude(contact.BC, contactNormalImpulse) * contact.BodyB->InvInertia;

		// .. \\ .. // .. \\ .. // .. \\ .. // .. \\ .. // .. \\ .. //
		// .. \\ .. // .. \\ .. // .. \\ .. // .. \\ .. // .. \\ .. //
		// .. \\ .. // .. \\ .. // .. \\ .. // .. \\ .. // .. \\ .. //

		// Relative velocity at contact (but the other way)
		relativeVelocity =
			contact.BodyB->Vel + Cross(contact.BodyB->AngularVel, contact.BC)
		  - contact.BodyA->Vel - Cross(contact.BodyA->AngularVel, contact.AC);

		//Calculate tangent.
		Vector2 tangent = Cross(contact.Normal, 1.0f);
		float vt = Dot(relativeVelocity, tangent);
		float dPt = contact.MassAtTangent * (-vt);

		// Compute friction impulse
		float friction = contact.BodyA->Friction * contact.BodyB->Friction;
		float maxPt = friction * contact.ImpulseSum;
		// Clamp friction
		float oldTangentImpulse = contact.FrictionSum;
		contact.FrictionSum = std::clamp(oldTangentImpulse + dPt, -maxPt, maxPt);
		dPt = contact.FrictionSum - oldTangentImpulse;

		Vector2 contactImpulse = tangent * dPt;
		contact.BodyA->Vel -= contactImpulse * contact.BodyA->InvMass;
		contact.BodyA->AngularVel -= CrossMagnitude(contact.AC, contactImpulse) * contact.BodyA->InvInertia;
		contact.BodyB->Vel += contactImpulse * contact.BodyB->InvMass;
		contact.BodyB->AngularVel += CrossMagnitude(contact.BC, contactImpulse) * contact.BodyB->InvInertia;
	}

}

void StaticVsDynamicCollisionSolver::Prestep(const float& inverseDeltaTime)
{
	size_t count = m_ContactPoints.size();
	float positionCorrection = 0.2f;

	for (size_t i = 0; i < count; i++)
	{
		ContactPoint& contact = m_ContactPoints[i];

		contact.AC = contact.Position - contact.BodyA->Pos;
		contact.BC = contact.Position - contact.BodyB->Pos;

		//Compute contact point Normal data
		float ACNormal = Dot(contact.AC, contact.Normal);
		float BCNormal = Dot(contact.BC, contact.Normal);

		float inverseMassSum = contact.BodyA->InvMass + contact.BodyB->InvMass;

		float massAtNormal = inverseMassSum;
		if(!contact.BodyA->IsStatic()) massAtNormal += contact.BodyA->InvInertia * (Dot(contact.AC, contact.AC) - ACNormal * ACNormal);
		if(!contact.BodyB->IsStatic()) massAtNormal += contact.BodyB->InvInertia * (Dot(contact.BC, contact.BC) - BCNormal * BCNormal);
		contact.MassAtNormal = 1.0f / massAtNormal;
		
		//Compute contact point tangential data
		Vector2 tangent = Cross(contact.Normal, 1.0f);
		float ACTangent = Dot(contact.AC, tangent);
		float BCTangent = Dot(contact.BC, tangent);

		float massAtTangent = inverseMassSum;
		if (!contact.BodyA->IsStatic()) massAtTangent += contact.BodyA->InvInertia * (Dot(contact.AC, contact.AC) - ACTangent * ACTangent);
		if (!contact.BodyB->IsStatic()) massAtTangent += contact.BodyB->InvInertia * (Dot(contact.BC, contact.BC) - BCTangent * BCTangent);
		contact.MassAtTangent = 1.0f / massAtTangent;

		contact.Bias = (-positionCorrection) * inverseDeltaTime * (std::min(0.0f, -contact.Depth + COLLISION_SKIN_DISTANCE));

		//Apply Normal + frictional contactImpulse to contact point.
		//Applying anti-pen contactImpulse
		Vector2 impulse = (contact.Normal * contact.ImpulseSum);
		//Applying frictional contactImpulse
		impulse += (tangent * contact.FrictionSum);

		if (contact.BodyA->IsStatic() == false)
		{
			contact.BodyA->Vel -= (impulse * contact.BodyA->InvMass);
			contact.BodyA->AngularVel -= (contact.BodyA->InvInertia * CrossMagnitude(contact.AC, impulse));
		}
		
		if (contact.BodyB->IsStatic() == false)
		{
			contact.BodyB->Vel += (impulse * contact.BodyB->InvMass);
			contact.BodyB->AngularVel -= (contact.BodyB->InvInertia * CrossMagnitude(contact.BC, impulse));
		}
	}
}

void StaticVsDynamicCollisionSolver::PhysicsStep()
{
	size_t count = m_ContactPoints.size();

	for (size_t i = 0; i < count; i++)
	{
		ContactPoint& contact = m_ContactPoints[i];

		//Relative velocity at contact.
		Vector2 relativeVelocity =
			contact.BodyB->Vel + Cross(contact.BodyB->AngularVel, contact.BC)
			- contact.BodyA->Vel - Cross(contact.BodyA->AngularVel, contact.AC);

		//Calculate Normal impulse
		float normalImpulse = Dot(relativeVelocity, contact.Normal);

		float dPn = contact.MassAtNormal * (-normalImpulse + contact.Bias);

		//Clamp accumulated impulse
		float clampedImpulse = contact.ImpulseSum;
		contact.ImpulseSum = std::max(clampedImpulse + dPn, 0.0f);
		dPn = contact.ImpulseSum - clampedImpulse;

		//Apply contact impulse
		Vector2 contactNormalImpulse = contact.Normal * dPn;
		if (contact.BodyA->IsStatic() == false)
		{
			contact.BodyA->Vel -= contactNormalImpulse * contact.BodyA->InvMass;
			contact.BodyA->AngularVel -= CrossMagnitude(contact.AC, contactNormalImpulse) * contact.BodyA->InvInertia;
		}

		if (contact.BodyB->IsStatic() == false)
		{
			contact.BodyB->Vel += contactNormalImpulse * contact.BodyB->InvMass;
			contact.BodyB->AngularVel += CrossMagnitude(contact.BC, contactNormalImpulse) * contact.BodyB->InvInertia;
		}

		// .. \\ .. // .. \\ .. // .. \\ .. // .. \\ .. // .. \\ .. //
		// .. \\ .. // .. \\ .. // .. \\ .. // .. \\ .. // .. \\ .. //
		// .. \\ .. // .. \\ .. // .. \\ .. // .. \\ .. // .. \\ .. //

		// Relative velocity at contact (but the other way)
		relativeVelocity =
			contact.BodyB->Vel + Cross(contact.BodyB->AngularVel, contact.BC)
			- contact.BodyA->Vel - Cross(contact.BodyA->AngularVel, contact.AC);

		//Calculate tangent.
		Vector2 tangent = Cross(contact.Normal, 1.0f);
		float vt = Dot(relativeVelocity, tangent);
		float dPt = contact.MassAtTangent * (-vt);

		// Compute friction impulse
		float friction = contact.BodyA->Friction * contact.BodyB->Friction;
		float maxPt = friction * contact.ImpulseSum;
		// Clamp friction
		float oldTangentImpulse = contact.FrictionSum;
		contact.FrictionSum = std::clamp(oldTangentImpulse + dPt, -maxPt, maxPt);
		dPt = contact.FrictionSum - oldTangentImpulse;

		Vector2 contactImpulse = tangent * dPt;
		if (contact.BodyA->IsStatic() == false)
		{
			contact.BodyA->Vel -= contactImpulse * contact.BodyA->InvMass;
			contact.BodyA->AngularVel -= CrossMagnitude(contact.AC, contactImpulse) * contact.BodyA->InvInertia;
		}

		if (contact.BodyB->IsStatic() == false)
		{
			contact.BodyB->Vel += contactImpulse * contact.BodyB->InvMass;
			contact.BodyB->AngularVel += CrossMagnitude(contact.BC, contactImpulse) * contact.BodyB->InvInertia;
		}
	}

}



void AlternateCollisionSolver::Prestep(const float& inverseDeltaTime)
{

}

void AlternateCollisionSolver::PhysicsStep()
{

}