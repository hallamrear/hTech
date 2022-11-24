#include "pch.h"
#include "Component_Rigidbody.h"
#include "Component_Script.h"
#include "Entity.h"
#include "PhysicsWorld.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "BoundingPolygon.h"
#include "OrientedBoundingBox.h"
#include "Game.h"

RigidbodyComponent::RigidbodyComponent(Entity& entity) : Component("Rigidbody Component", entity)
{
	mIsStatic = false;
	mGravityEnabled = true;
	mDragEnabled = true;
	mMass = 100.0f; 
	mDragCoefficient = 0.47f;
	mSpeedCap = 1000.0f;
	mRestitution = 0.5f;
	mCollider = nullptr;

	CalculateInverseMass();

	Physics::RegisterRigidbody(this);
}

RigidbodyComponent::~RigidbodyComponent()
{
	Physics::DeregisterRigidbody(this);

	mGravityEnabled  = false;
	mDragEnabled	 = false;
	mMass			 = 0.0f;
	mInverseMass	 = 0.0f;
	mDragCoefficient = 0.0f;
	mSpeedCap		 = 0.0f;
	mRestitution	 = 0.0f;
	mVelocity		 = Vector2();
	mAcceleration	 = Vector2();
	mNetForce		 = Vector2();
	mExternalForce	 = Vector2();
	mIsStatic = false;
}

/// <summary>
/// Do not call this function. It will mess up the physics timestep calculations.
/// </summary>
/// <param name="DeltaTime"></param>
void RigidbodyComponent::PhysicsUpdate(float DeltaTime)
{
	if (mIsStatic)
		return;

	if (GetDragEnabled())
	{
		///Drag
		Vector2 dragForce;
		dragForce.X = -mDragCoefficient * mVelocity.X;
		dragForce.Y = -mDragCoefficient * mVelocity.Y;
		mNetForce += dragForce;

		Vector2 frictionForce;
		float mew = 2.5f;
		if (mVelocity.GetMagnitude() < mew * DeltaTime) // Make sure the friction doesn't overextend.
			frictionForce = ((mVelocity * -1) / DeltaTime);
		else
			frictionForce = (mVelocity.GetNormalized() * -1) * mew; // getNormalized() returns the unit vector.

		frictionForce = ((mVelocity * -1) * mew);

		mNetForce += frictionForce;
	}

	///External
	//No need for gravity
	if (GetGravityEnabled())
	{
		mNetForce += (Settings::Get()->GetGravityDirection() * mMass);
	}

	mNetForce += mExternalForce;

	///Acceleration
	mAcceleration = mNetForce / mMass;

	///Update Position
	mVelocity += mAcceleration * static_cast<float>(DeltaTime);
	Parent.GetTransform().Position += mVelocity;

	///Speed Cap
	//Capping - X
	if (mVelocity.X > mSpeedCap)
		mVelocity.X = mSpeedCap;
	else if (mVelocity.X < -mSpeedCap)
		mVelocity.X = -mSpeedCap;
	//Capping - Y
	if (mVelocity.Y > mSpeedCap)
		mVelocity.Y = mSpeedCap;
	else if (mVelocity.Y < -mSpeedCap)
		mVelocity.Y = -mSpeedCap;

	mNetForce = Vector2();
	mExternalForce = Vector2();
}

void RigidbodyComponent::DestroyCollider()
{
	if (mCollider)
	{
		delete mCollider;
		mCollider = nullptr;
	}
}

void RigidbodyComponent::SetCollider(COLLIDER_TYPE type)
{
	if (mCollider != nullptr)
	{
		if (type != mCollider->mType)
		{
			DestroyCollider();
		}
	}

	switch (type)
	{
	default:
	case COLLIDER_TYPE::COLLIDER_UNKNOWN:
		--mCollider = nullptr;
		break;
	case COLLIDER_TYPE::COLLIDER_AABB:
		mCollider = new BoundingBox(Parent.GetTransform(), 64, 64);
		break;
	case COLLIDER_TYPE::COLLIDER_SPHERE:
		mCollider = new BoundingSphere(Parent.GetTransform(), 64);
		break;
	case COLLIDER_TYPE::COLLIDER_OBB:
		mCollider = new OrientedBoundingBox(Parent.GetTransform(), 64, 64);
		break;
	case COLLIDER_TYPE::COLLIDER_POLYGON:
		//TODO : Implement
		break;
	}
}

Collider* const RigidbodyComponent::GetCollider()
{
	if (mCollider != nullptr)
	{
		return mCollider;
	}
	else
	{
		return nullptr;
	}
}

void RigidbodyComponent::AddForce(Vector2 force)
{
	mExternalForce += force;
}

void RigidbodyComponent::AddForce(float X, float Y)
{
	mExternalForce.X += X;
	mExternalForce.Y += Y;
}

void RigidbodyComponent::CalculateInverseMass()
{
	if (GetIsStatic())
		mInverseMass = 0.0f;
	else
		mInverseMass = 1.0f / mMass;
}

void RigidbodyComponent::OnCollision(const CollisionManifold& manifold, RigidbodyComponent& other)
{
	ScriptComponent* script = Parent.GetComponent<ScriptComponent>();
	if (script != nullptr)
	{
		script->OnCollision(manifold, other);
	}
}

void RigidbodyComponent::OnOverlap(const CollisionManifold& manifold, RigidbodyComponent& other)
{
	ScriptComponent* script = Parent.GetComponent<ScriptComponent>();
	if (script != nullptr)
	{
		script->OnOverlap(manifold, other);
	}
}

void RigidbodyComponent::Update(float DeltaTime)
{
	if (mCollider)
	{
		mCollider->Update(DeltaTime);
	}
}

void RigidbodyComponent::Render(SDL_Renderer& renderer)
{
	if (mCollider)
	{
		mCollider->Render(renderer);
	}
}