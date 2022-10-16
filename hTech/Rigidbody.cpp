#include "pch.h"
#include "Rigidbody.h"
#include "PhysicsWorld.h"
#include "Log.h"
#include "Texture.h"
#include "Game.h"

Rigidbody::Rigidbody(std::string texture_path = "", PhysicsProperties properties = PhysicsProperties())
	: Entity(texture_path)
{
	Physics::RegisterRigidbody(this);

	mIsStatic = properties.IsStatic;
	mGravityEnabled = properties.GravityEnabled;
	mDragEnabled = properties.DragEnabled;
	mMass = properties.Mass;
	mDragCoefficient = properties.DragCoefficient;
	mSpeedCap = properties.SpeedCap;
	mRestitution = properties.Restitution;

	mVelocity = Vector2();
	mAcceleration = Vector2();
	mNetForce = Vector2();
	mExternalForce = Vector2();

	CalculateInverseMass();
}

void Rigidbody::AddForce(Vector2 force)
{
	mExternalForce += force;
}

void Rigidbody::AddForce(float X, float Y)
{
	mExternalForce.X += X;
	mExternalForce.Y += Y;
}

void Rigidbody::CalculateInverseMass()
{
	if (GetIsStatic())
		mInverseMass = 0.0f;
	else
		mInverseMass = 1.0f / mMass;
}

Rigidbody::~Rigidbody()
{
	Physics::DeregisterRigidbody(this);
}

void Rigidbody::PhysicsUpdate(double deltaTime)
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
		if (mVelocity.GetMagnitude() < mew * deltaTime) // Make sure the friction doesn't overextend.
			frictionForce = ((mVelocity * -1) / (float)deltaTime);
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
	mVelocity += mAcceleration * static_cast<float>(deltaTime);
	GetTransform().Position += mVelocity;

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

void Rigidbody::Update(double deltaTime)
{
	if (mIsWaitingToBeDestroyed == false)
	{

	}
}

void Rigidbody::Render()
{
	if (mIsWaitingToBeDestroyed == false)
	{
		if (mTexture)
		{
			mTexture->Render(*Game::Renderer, GetTransform().Position, GetTransform().Rotation);
		}

		if (mCollider)
		{
			mCollider->Render(*Game::Renderer);
		}
	}
}

void Rigidbody::OnCollision(const CollisionManifold& manifold, Rigidbody& other)
{

}

void Rigidbody::OnOverlap(const CollisionManifold& manifold, Rigidbody& other)
{

}
