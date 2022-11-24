#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include "Collision.h"

#define FIXED_TIME_STEP (1.0f / 60.0f)
#define IMPULSE_ITERATION_COUNT 1

class HTECH_FUNCTION_EXPORT RigidbodyComponent;

class HTECH_FUNCTION_EXPORT Physics
{
	static Physics* mInstance;
	float mFixedTimestep;
	std::vector<RigidbodyComponent*> mRigidbodies;
	std::vector<CollisionManifold> mManifolds;

	Physics(float fixedTimeStep);
	~Physics();
	
	void FixedUpdate();
	void Update_Impl(float DeltaTime);
	void RegisterRigidbody_Impl(RigidbodyComponent* rb);
	void DeregisterRigidbody_Impl(RigidbodyComponent* rb);

protected:
	static Physics* Get();

public:
	
	static void Update(float DeltaTime);
	static void RegisterRigidbody(RigidbodyComponent* rb);
	static void DeregisterRigidbody(RigidbodyComponent* rb);
};

