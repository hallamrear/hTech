#include "pch.h"
#include "Particle.h"
#include "Camera.h"
#include "Component_ParticleGen.h"
#include "Component_Rigidbody.h"
#include "Collision.h"

Particle::Particle(ParticleSystemComponent& emitter) : mEmitter(emitter)
{
	mLifespan = 0.0f;
	AddComponent<RigidbodyComponent>();
	RigidbodyComponent* component = GetComponent<RigidbodyComponent>();
	component->SetCollider(COLLIDER_TYPE::COLLIDER_AABB);
	component->GetCollider()->IsOverlap = true;
}

Particle::~Particle()
{

}

void Particle::Reset()
{
	mLifespan = 0.0f;
	mIsAlive = true;
	GetTransform().Position = mEmitter.GetEntity().GetTransform().Position;
	GetComponent<RigidbodyComponent>()->Reset();

	Vector2 dir;
	dir.X = rand() % 16 - 8;
	dir.Y = rand() % 16 - 8;
	GetComponent<RigidbodyComponent>()->AddVelocity(dir);
}

void Particle::Render(SDL_Renderer& renderer, const ParticleGenDetails& details)
{
	if (mIsAlive)
	{
		GetComponent<RigidbodyComponent>()->GetCollider()->Render(renderer);

		Vector2 screenPos = Camera::WorldToScreen(GetTransform().Position);
		SDL_Rect rect{ (int)screenPos.X, (int)screenPos.Y, details.width, details.height };
		SDL_SetRenderDrawColor(&renderer, details.colour.R, details.colour.G, details.colour.B, details.colour.A);
		SDL_SetRenderDrawColor(&renderer, rand() % 255, rand() % 255, rand() % 255, details.colour.A);
		
		SDL_RenderDrawRect(&renderer, &rect);
	}
}

void Particle::Update(float deltaTime, const ParticleGenDetails& details)
{
	if (mIsAlive)
	{
		mLifespan += deltaTime;

		if (mLifespan > details.duration)
		{
			mIsAlive = false;
		}
	}
}
