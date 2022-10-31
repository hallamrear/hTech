#include "pch.h"
#include "PhysicsWorld.h"
#include "Component_Rigidbody.h"

Physics* Physics::mInstance = nullptr;

Physics::Physics(float fixedTimeStep)
{
	mFixedTimestep = fixedTimeStep;
}

Physics::~Physics()
{
	mRigidbodies.clear();
}

void Physics::FixedUpdate()
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

			if (mRigidbodies[i]->HasCollider() && mRigidbodies[j]->HasCollider() && mRigidbodies[i]->GetIsEnabled() && mRigidbodies[j]->GetIsEnabled())
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

	for (size_t i = 0; i < manifoldCount; i++)
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
	{
		if (itr->GetIsEnabled() == true)
		{
			itr->PhysicsUpdate(mFixedTimestep);
		}
	}
}

void Physics::Update_Impl(float DeltaTime)
{
	FixedUpdate();
}

void Physics::Update(float DeltaTime)
{
	Get()->Update_Impl(DeltaTime);
}

void Physics::RegisterRigidbody(RigidbodyComponent* rb)
{
	Get()->RegisterRigidbody_Impl(rb);
}

void Physics::RegisterRigidbody_Impl(RigidbodyComponent* rb)
{
	mRigidbodies.push_back(rb);
}

void Physics::DeregisterRigidbody(RigidbodyComponent* rb)
{
	Get()->DeregisterRigidbody_Impl(rb);
}

void Physics::DeregisterRigidbody_Impl(RigidbodyComponent* rb)
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

Physics* Physics::Get()
{
	if (!mInstance)
		mInstance = new Physics(FIXED_TIME_STEP);

	return mInstance;
}