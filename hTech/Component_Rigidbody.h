#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "Component.h"

enum class COLLIDER_TYPE : int;

class HTECH_FUNCTION_EXPORT CollisionManifold;
class HTECH_FUNCTION_EXPORT Collider;

class HTECH_FUNCTION_EXPORT RigidbodyComponent :
    public Component
{
private:

protected:
	Collider*				m_Collider;
	bool					m_GravityEnabled;
	bool					m_DragEnabled;
	float					m_Mass; //Weight of Entity (kg)
	float					m_InverseMass;
	float					m_DragCoefficient;
	float					m_SpeedCap; //Speed cap (u/s)
	float					m_Restitution;
	Vector2					m_Velocity;
	Vector2					m_Acceleration;
	Vector2					m_NetForce;
	Vector2					m_ExternalForce;
	bool					m_IsStatic;

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

	Vector2 const	GetVelocity()		const { return m_Velocity; }
	Vector2 const	GetAcceleration()	const { return m_Acceleration; }
	bool	const	GetGravityEnabled() const { return m_GravityEnabled; }
	bool	const	GetDragEnabled()	const { return m_DragEnabled; }
	float	const	GetMass()			const { return m_Mass; };
	float	const	GetInverseMass()	const { return m_InverseMass; };
	float	const	GetRestitution()	const { return m_Restitution; };
	bool	const	GetIsStatic()		const { return m_IsStatic; };
	bool	const	HasCollider()		const { return (m_Collider != nullptr); }

	void		    SetCollider(COLLIDER_TYPE type);
	Collider* const GetCollider();

	//Setters
	virtual void	SetIsStatic(const bool state) { m_IsStatic = state; }
	virtual void	SetGravityEnabled(const bool state) { m_GravityEnabled = state; }
	virtual void	SetDragEnabled(const bool state) { m_DragEnabled = state; }
	//Adjusters
	virtual void	AddVelocity(const Vector2 velocity) { m_Velocity += velocity; };
	virtual void	AddExternalForce(const Vector2 force) { m_ExternalForce += force; };

	virtual void	OnCollision(const CollisionManifold& manifold, RigidbodyComponent& other);
	virtual void	OnOverlap(const CollisionManifold& manifold, RigidbodyComponent& other);

	void			Update(float DeltaTime);
	void			Render(SDL_Renderer& renderer);

	void Serialize(Serializer& writer) const override;
	void Deserialize(SerializedValue& value) override;
};