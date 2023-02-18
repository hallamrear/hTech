#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "Collision.h"
#include "Vector2.h"

#define FIXED_TIME_STEP (1.0f / 60.0f)
#define IMPULSE_ITERATION_COUNT 1

class HTECH_FUNCTION_EXPORT RigidbodyComponent;
class HTECH_FUNCTION_EXPORT CollisionSolver;

class HTECH_FUNCTION_EXPORT Physics
{
	Vector2 m_Gravity;
	static Physics* m_Instance;
	float mFixedTimestep;
	std::vector<RigidbodyComponent*> m_RigidbodyVector;
	std::vector<CollisionSolver*> m_PhysicsSolvers;

	Physics(float fixedTimeStep);
	~Physics();
	
	void FixedUpdate();
	void Update_Impl(float DeltaTime);
	void RegisterRigidbody_Impl(RigidbodyComponent* rb);
	void DeregisterRigidbody_Impl(RigidbodyComponent* rb);
	void SetGravity_Impl(const Vector2& gravity);
	const Vector2 GetGravity_Impl();

protected:
	static Physics* Get();

public:
	static void SetGravity(const Vector2& gravity);
	static const Vector2 GetGravity();
	static void Update(float DeltaTime);
	static void RegisterRigidbody(RigidbodyComponent* rb);
	static void DeregisterRigidbody(RigidbodyComponent* rb);
};

