#pragma once
#include "Entity.h"
#include "PhysicsProperties.h"

/*

class CollisionManifold;

class Rigidbody : public Entity
{
private:
	Rigidbody(
		std::string texture_path,
		PhysicsProperties properties);


protected:
	bool					mGravityEnabled;
	bool					mDragEnabled;
	float					mMass; //Weight of Entity (kg)
	float					mInverseMass;
	float					mDragCoefficient;
	float					mSpeedCap; //Speed cap (u/s)
	float					mRestitution;
	Vector2					mVelocity;
	Vector2					mAcceleration;
	Vector2					mNetForce;
	Vector2					mExternalForce;
	Collider*				mCollider;
	bool					mIsStatic;

	//Recalculates the inverse mass based on the current mass
	void					CalculateInverseMass();
	void					AddForce(Vector2 force);
	void					AddForce(float X = 0.0f, float Y = 0.0f);

public:
					~Rigidbody();

	//Do not call this outside of physics world.
	virtual void	 PhysicsUpdate(float DeltaTime);

	/// <summary>
	/// Overrideable
	/// </summary>
	/// <param name="DeltaTime"></param>
	virtual void	 Update(float DeltaTime);

	/// <summary>
	/// Overrideable
	/// </summary>
	virtual void	 Render();

	virtual void	 OnCollision(const CollisionManifold& manifold, Rigidbody& other);
	virtual void	 OnOverlap(const CollisionManifold& manifold, Rigidbody& other);

	Vector2   const GetVelocity()		 const { return mVelocity; }
	Vector2   const GetAcceleration()	 const { return mAcceleration; }
	bool	   const GetGravityEnabled() const { return mGravityEnabled; }
	bool	   const GetDragEnabled()	 const { return mDragEnabled; }
	float	   const GetMass()			 const { return mMass; };
	float	   const GetInverseMass()	 const { return mInverseMass; };
	float	   const GetRestitution()	 const { return mRestitution; };
	bool	   const GetIsStatic()		 const { return mIsStatic; };
	Collider*  const GetCollider()		 const { return mCollider; };
	//Setters
	virtual void	SetGravityEnabled(const bool state) { mGravityEnabled = state; }
	virtual void	SetDragEnabled(const bool state) { mDragEnabled = state; }
	//Adjusters
	virtual void	AddVelocity(const Vector2 velocity) { mVelocity += velocity; };
	virtual void	AddExternalForce(const Vector2 force) { mExternalForce += force; };
};

*/