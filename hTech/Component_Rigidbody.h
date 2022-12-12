#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "Component.h"

enum COLLIDER_TYPE;
class HTECH_FUNCTION_EXPORT CollisionManifold;
class HTECH_FUNCTION_EXPORT Collider;

class HTECH_FUNCTION_EXPORT RigidbodyComponent :
    public Component
{
private:

protected:
	Collider*				mCollider;
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
	bool					mIsStatic;

	//Recalculates the inverse mass based on the current mass
	void					CalculateInverseMass();
	void					AddForce(Vector2 force);
	void					AddForce(float X = 0.0f, float Y = 0.0f);
	void					RenderProperties();

public:
    RigidbodyComponent(Entity& entity);
    ~RigidbodyComponent();

	//Do not call this outside of physics world.
	virtual void	PhysicsUpdate(float DeltaTime);

	void DestroyCollider();

	Vector2 const	GetVelocity()		const { return mVelocity; }
	Vector2 const	GetAcceleration()	const { return mAcceleration; }
	bool	const	GetGravityEnabled() const { return mGravityEnabled; }
	bool	const	GetDragEnabled()	const { return mDragEnabled; }
	float	const	GetMass()			const { return mMass; };
	float	const	GetInverseMass()	const { return mInverseMass; };
	float	const	GetRestitution()	const { return mRestitution; };
	bool	const	GetIsStatic()		const { return mIsStatic; };
	bool	const	HasCollider()		const { return (mCollider != nullptr); }

	void		    SetCollider(COLLIDER_TYPE type);
	Collider* const GetCollider();

	//Setters
	virtual void	SetIsStatic(const bool state) { mIsStatic = state; }
	virtual void	SetGravityEnabled(const bool state) { mGravityEnabled = state; }
	virtual void	SetDragEnabled(const bool state) { mDragEnabled = state; }
	//Adjusters
	virtual void	AddVelocity(const Vector2 velocity) { mVelocity += velocity; };
	virtual void	AddExternalForce(const Vector2 force) { mExternalForce += force; };

	virtual void	OnCollision(const CollisionManifold& manifold, RigidbodyComponent& other);
	virtual void	OnOverlap(const CollisionManifold& manifold, RigidbodyComponent& other);

	void			Update(float DeltaTime);
	void			Render(SDL_Renderer& renderer);

	void Serialize(Serializer& writer) const override;
	void Deserialize(SerializedValue& value) override;
};