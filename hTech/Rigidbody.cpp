#include "pch.h"
#include "Rigidbody.h"
#include "PhysicsWorld.h"
#include "Log.h"

Rigidbody::Rigidbody(std::string texture_path = "",	Transform transform = Transform(), PhysicsProperties properties = PhysicsProperties())
	: Entity(texture_path, transform)
{
	PhysicsWorld::RegisterRigidbody(this);

	mIsStatic = properties.IsStatic;
	mGravityEnabled = properties.GravityEnabled;
	mDragEnabled = properties.DragEnabled;
	mMass = properties.Mass;
	mDragCoefficient = properties.DragCoefficient;
	mSpeedCap = properties.SpeedCap;
	mRestitution = properties.Restitution;

	mVelocity = Vector2f();
	mAcceleration = Vector2f();
	mNetForce = Vector2f();
	mExternalForce = Vector2f();

	CalculateInverseMass();
}

void Rigidbody::AddForce(Vector2f force)
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
	PhysicsWorld::DeregisterRigidbody(this);
}

void Rigidbody::PhysicsUpdate(double deltaTime)
{
	if (mIsStatic)
		return;

	if (GetDragEnabled())
	{
		///Drag
		Vector2f dragForce;
		dragForce.X = -mDragCoefficient * mVelocity.X;
		dragForce.Y = -mDragCoefficient * mVelocity.Y;
		mNetForce += dragForce;

		Vector2f frictionForce;
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
		mNetForce += (Settings::Get()->GetGravityDirection() * mMass);

	mNetForce += mExternalForce;

	///Acceleration
	mAcceleration = mNetForce / mMass;

	///Update Position
	mVelocity += mAcceleration * static_cast<float>(deltaTime);
	mTransform.Position += mVelocity;

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

	mNetForce = Vector2f();
	mExternalForce = Vector2f();
}

void Rigidbody::OnCollision(const CollisionManifold& manifold, Rigidbody& other)
{

}

void Rigidbody::OnOverlap(const CollisionManifold& manifold, Rigidbody& other)
{

}
