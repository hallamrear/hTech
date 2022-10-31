#pragma once
#include "Collision.h"

#define FIXED_TIME_STEP (1.0 / 60.0)
#define IMPULSE_ITERATION_COUNT 1

class RigidbodyComponent;

class Physics
{
	static Physics* mInstance;
	double mFixedTimestep;
	std::vector<RigidbodyComponent*> mRigidbodies;
	std::vector<CollisionManifold> mManifolds;

	Physics(double fixedTimeStep);
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

