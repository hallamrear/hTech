#include "pch.h"
#include "PhysicsWorld.h"
#include "Component_Rigidbody.h"

Physics* Physics::m_Instance = nullptr;

Physics::Physics(float fixedTimeStep)
{
	mFixedTimestep = fixedTimeStep;
}

Physics::~Physics()
{
	m_RigidbodyVector.clear();
}

void Physics::FixedUpdate()
{
	m_ManifoldVector.clear();

	size_t rbCount = m_RigidbodyVector.size();

	//Find our collisions
	for (size_t i = 0; i < rbCount; i++)
	{
		for (size_t j = i; j < rbCount; j++)
		{
			if (i == j)
				continue;

			CollisionManifold manifold;

			if (m_RigidbodyVector[i]->HasCollider() && m_RigidbodyVector[j]->HasCollider() && m_RigidbodyVector[i]->GetIsEnabled() && m_RigidbodyVector[j]->GetIsEnabled())
			{
				if (Collision::CheckCollision(*m_RigidbodyVector[i]->GetCollider(), *m_RigidbodyVector[j]->GetCollider(), &manifold))
				{
					manifold.ObjA = m_RigidbodyVector[i];
					manifold.ObjB = m_RigidbodyVector[j];
					m_ManifoldVector.push_back(manifold);

					if (m_RigidbodyVector[i]->GetCollider()->IsOverlap())
						m_RigidbodyVector[i]->OnOverlap(manifold, *m_RigidbodyVector[j]);
					else
						m_RigidbodyVector[i]->OnCollision(manifold, *m_RigidbodyVector[j]);

					if (m_RigidbodyVector[j]->GetCollider()->IsOverlap())
						m_RigidbodyVector[j]->OnOverlap(manifold, *m_RigidbodyVector[i]);
					else
						m_RigidbodyVector[j]->OnCollision(manifold, *m_RigidbodyVector[i]);
				}
			}
		}
	}

	//Resolve our collisions via iterative impulse resolution
	size_t manifoldCount = m_ManifoldVector.size();
	CollisionManifold* currentManifold = nullptr;

	for (size_t i = 0; i < manifoldCount; i++)
	{
		currentManifold = &m_ManifoldVector[i];

		for (int k = 0; k < IMPULSE_ITERATION_COUNT; k++)
		{
			//todo : add should resolve to rigidbody to allow for triggers.
			if(!(currentManifold->ObjA->GetCollider()->IsOverlap() || currentManifold->ObjB->GetCollider()->IsOverlap()))
				Collision::ResolveCollision(*currentManifold->ObjA, *currentManifold->ObjB, currentManifold);
		}
	}
	
	//Physics update
	for (auto& itr : m_RigidbodyVector)
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
	m_RigidbodyVector.push_back(rb);
}

void Physics::DeregisterRigidbody(RigidbodyComponent* rb)
{
	Get()->DeregisterRigidbody_Impl(rb);
}

void Physics::DeregisterRigidbody_Impl(RigidbodyComponent* rb)
{
	if (m_RigidbodyVector.size() > 0)
	{
		auto itr = std::find(m_RigidbodyVector.begin(), m_RigidbodyVector.end(), rb);

		if (itr != m_RigidbodyVector.end())
		{
			m_RigidbodyVector.erase(itr);
		}
		else	
			assert(itr == m_RigidbodyVector.end());
	}
}

Physics* Physics::Get()
{
	if (!m_Instance)
		m_Instance = new Physics(FIXED_TIME_STEP);

	return m_Instance;
}