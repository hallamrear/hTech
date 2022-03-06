#include "pch.h"
#include "PhysicsWorld.h"
#include "Rigidbody.h"

PhysicsWorld* PhysicsWorld::mInstance = nullptr;

PhysicsWorld::PhysicsWorld(double fixedTimeStep)
{
	mFixedTimestep = fixedTimeStep;
}

PhysicsWorld::~PhysicsWorld()
{
	mRigidbodies.clear();
}

void PhysicsWorld::FixedUpdate()
{
	mManifolds.clear();

	size_t rbCount = mRigidbodies.size();

	//Find our collisions
	for (size_t i = 0; i < rbCount; i++)
	{
		for (size_t j = i; j < rbCount; j++)
		{
			if (i == j)
				continue;

			CollisionManifold manifold;

			if (mRigidbodies[i]->GetCollider() && mRigidbodies[j]->GetCollider() && mRigidbodies[i]->GetIsAlive() && mRigidbodies[j]->GetIsAlive())
			{
				if (Collision::CheckCollision(*mRigidbodies[i]->GetCollider(), *mRigidbodies[j]->GetCollider(), &manifold))
				{
					manifold.ObjA = mRigidbodies[i];
					manifold.ObjB = mRigidbodies[j];
					mManifolds.push_back(manifold);

					if (mRigidbodies[i]->GetCollider()->IsOverlap)
						mRigidbodies[i]->OnOverlap(manifold, *mRigidbodies[j]);
					else
						mRigidbodies[i]->OnCollision(manifold, *mRigidbodies[j]);

					if (mRigidbodies[j]->GetCollider()->IsOverlap)
						mRigidbodies[j]->OnOverlap(manifold, *mRigidbodies[i]);
					else
						mRigidbodies[j]->OnCollision(manifold, *mRigidbodies[i]);
				}
			}
		}
	}

	//Resolve our collisions via iterative impulse resolution
	size_t manifoldCount = mManifolds.size();
	CollisionManifold* currentManifold = nullptr;

	for (int i = 0; i < manifoldCount; i++)
	{
		currentManifold = &mManifolds[i];

		for (int k = 0; k < IMPULSE_ITERATION_COUNT; k++)
		{
			//todo : add should resolve to rigidbody to allow for triggers.
			if(!(currentManifold->ObjA->GetCollider()->IsOverlap || currentManifold->ObjB->GetCollider()->IsOverlap))
				Collision::ResolveCollision(*currentManifold->ObjA, *currentManifold->ObjB, currentManifold);
		}
	}
	
	//Physics update
	for (auto& itr : mRigidbodies)
		itr->PhysicsUpdate(mFixedTimestep);
}

void PhysicsWorld::Update_Impl(double deltaTime)
{
	FixedUpdate();
}

void PhysicsWorld::Update(double deltaTime)
{
	Get()->Update_Impl(deltaTime);
}

void PhysicsWorld::RegisterRigidbody(Rigidbody* rb)
{
	Get()->RegisterRigidbody_Impl(rb);
}

void PhysicsWorld::RegisterRigidbody_Impl(Rigidbody* rb)
{
	mRigidbodies.push_back(rb);
}

void PhysicsWorld::DeregisterRigidbody(Rigidbody* rb)
{
	Get()->DeregisterRigidbody_Impl(rb);
}

void PhysicsWorld::DeregisterRigidbody_Impl(Rigidbody* rb)
{
	if (mRigidbodies.size() > 0)
	{
		auto itr = std::find(mRigidbodies.begin(), mRigidbodies.end(), rb);

		if (itr != mRigidbodies.end())
		{
			mRigidbodies.erase(itr);
		}
		else	
			assert(itr == mRigidbodies.end());
	}
}

PhysicsWorld* PhysicsWorld::Get()
{
	if (!mInstance)
		mInstance = new PhysicsWorld(FIXED_TIME_STEP);

	return mInstance;
}