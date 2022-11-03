#include "pch.h"
#include "Component_ParticleGen.h"
#include "Entity.h"

ParticleSystemComponent::ParticleSystemComponent(Entity& entity, ParticleGenDetails details) : Component("Particle Generator", entity)
{
	mParticleSystemDetails = details;
	mIsRunning = details.playOnStart;

	for (size_t i = 0; i < PARTICLE_COUNT; i++)
	{
		mParticles[i] = new Particle(*this);
	}
}

ParticleSystemComponent::~ParticleSystemComponent()
{
	for (size_t i = 0; i < PARTICLE_COUNT; i++)
	{
		delete mParticles[i];
		mParticles[i] = nullptr;
	}
}

void ParticleSystemComponent::SetDetails(ParticleGenDetails details)
{
	mParticleSystemDetails = details;
	mIsRunning = details.playOnStart;
}

ParticleGenDetails& ParticleSystemComponent::GetDetails()
{
	return mParticleSystemDetails;
}

void ParticleSystemComponent::Update(float deltaTime)
{
	if (mIsRunning)
	{
		for (size_t i = 0; i < PARTICLE_COUNT; i++)
		{
			if (mParticles[i]->GetIsAlive())
			{
				mParticles[i]->Update(deltaTime, mParticleSystemDetails);
			}
			else
			{
				if (mParticleSystemDetails.looping == true)
				{
					mParticles[i]->Reset();
				}
			}
		}
	}
}

void ParticleSystemComponent::Render(SDL_Renderer& renderer)
{
	if (mIsRunning)
	{
		for (size_t i = 0; i < PARTICLE_COUNT; i++)
		{
			if (mParticles[i]->GetIsAlive())
			{				 
				mParticles[i]->Render(renderer, mParticleSystemDetails);
			}
		}
	}
}

void ParticleSystemComponent::Play()
{
	mIsRunning = true;
	for (size_t i = 0; i < PARTICLE_COUNT; i++)
	{
		mParticles[i]->Reset();
	}
}

void ParticleSystemComponent::Pause()
{
	mIsRunning = false;
}