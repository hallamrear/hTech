#include "pch.h"
#include "PhysicsWorld.h"
#include "GJKCollisionSolver.h"
#include "CollisionManifold.h"
#include "Component_Rigidbody.h"
#include "Collider.h"

Physics* Physics::m_Instance = nullptr;

Physics::Physics(float fixedTimeStep)
{
	mFixedTimestep = fixedTimeStep;
}

Physics::~Physics()
{
	for (size_t i = 0; i < m_PhysicsSolvers.size(); i++)
	{
		delete m_PhysicsSolvers[i];
		m_PhysicsSolvers[i] = nullptr;
	}
	m_PhysicsSolvers.clear();

	m_RigidbodyVector.clear();
}

void Physics::FixedUpdate()
{
	size_t rbCount = m_RigidbodyVector.size();

	//Find our collisions
	for (size_t i = 0; i < rbCount; i++)
	{
		for (size_t j = i + 1; j < rbCount; j++)
		{
			if (i == j)
				continue;

			CollisionManifold manifold;

			if (m_RigidbodyVector[i]->HasCollider() && m_RigidbodyVector[j]->HasCollider() && m_RigidbodyVector[i]->GetIsEnabled() && m_RigidbodyVector[j]->GetIsEnabled())
			{
				if (Collision::CheckCollision(*m_RigidbodyVector[i], *m_RigidbodyVector[j], &manifold))
				{
					if (m_RigidbodyVector[i]->GetCollider()->IsOverlap())
					{
						if (m_RigidbodyVector[i]->GetCollider()->IsOverlap())
							m_RigidbodyVector[i]->OnOverlap(manifold, *m_RigidbodyVector[j]);
					}
					else if (m_RigidbodyVector[j]->GetCollider()->IsOverlap())
					{
						if (m_RigidbodyVector[j]->GetCollider()->IsOverlap())
							m_RigidbodyVector[j]->OnOverlap(manifold, *m_RigidbodyVector[i]);
					}
					else
					{
						if ((m_RigidbodyVector[i]->GetIsStatic() && m_RigidbodyVector[j]->GetIsStatic()) == false)
						{
							manifold.BodyA = m_RigidbodyVector[i];
							manifold.BodyB = m_RigidbodyVector[j];
							m_PhysicsSolvers.push_back(new GJKCollisionSolver(manifold));
							m_RigidbodyVector[i]->OnCollision(manifold, *m_RigidbodyVector[j]);
							m_RigidbodyVector[j]->OnCollision(manifold, *m_RigidbodyVector[i]);
						}
					}
				}
			}
		}
	}

	//Resolve our collisions via iterative impulse resolution
	size_t solverCount = m_PhysicsSolvers.size();
	///Perform Physics Pre-step calculations
	for (size_t i = 0; i < solverCount; i++)
	{
		m_PhysicsSolvers[i]->Prestep();
	}

	///Perform Physics Iterations
	for (int k = 0; k < IMPULSE_ITERATION_COUNT; k++)
	{
		for (size_t i = 0; i < solverCount; i++)
		{
			m_PhysicsSolvers[i]->PhysicsStep();

			delete m_PhysicsSolvers[i];
			m_PhysicsSolvers[i] = nullptr;
		}
	}

	m_PhysicsSolvers.clear();
	
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