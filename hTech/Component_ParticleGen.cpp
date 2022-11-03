#include "pch.h"
#include "Component_ParticleGen.h"
#include "Entity.h"

ParticleSystemComponent::ParticleSystemComponent(Entity& entity, ParticleGenDetails details) : Component("Particle Generator", entity)
{
	mParticleSystemDetails = details;
	mIsRunning = details.playOnStart;
}

ParticleSystemComponent::~ParticleSystemComponent()
{

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
			if (mParticles[i].GetIsAlive())
			{
				mParticles->Update(deltaTime, mParticleSystemDetails);
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
			if (mParticles[i].GetIsAlive())
			{
				mParticles->Render(renderer, mParticleSystemDetails);
			}
		}
	}
}

void ParticleSystemComponent::Play()
{
	mIsRunning = true;
	for (size_t i = 0; i < PARTICLE_COUNT; i++)
	{
		mParticles->Reset();
	}
}

void ParticleSystemComponent::Pause()
{
	mIsRunning = false;
}