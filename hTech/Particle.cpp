#include "pch.h"
#include "Particle.h"
#include "Camera.h"
#include "Component_ParticleGen.h"
#include "Component_Rigidbody.h"
#include "BoundingBox.h"

Particle::Particle(ParticleSystemComponent& emitter) : mEmitter(emitter)
{
	mLifespan = 0.0f;
	AddComponent<RigidbodyComponent>();
	RigidbodyComponent* component = GetComponent<RigidbodyComponent>();
	component->SetCollider(COLLIDER_TYPE::COLLIDER_AABB);
	component->GetCollider()->IsOverlap = true;
	mCollider = (BoundingBox*)GetComponent<RigidbodyComponent>()->GetCollider();
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
	dir.Y = rand() % 25 - 8;
	GetComponent<RigidbodyComponent>()->AddVelocity(dir);
}

void Particle::Render(SDL_Renderer& renderer, const ParticleGenDetails& details)
{
	if (mIsAlive)
	{
		Vector2 screenPos = Camera::WorldToScreen(GetTransform().Position);
		SDL_Rect rect{ (int)screenPos.X, (int)screenPos.Y, details.width, details.height };
		SDL_SetRenderDrawColor(&renderer, details.colour.R, details.colour.G, details.colour.B, details.colour.A);
		SDL_RenderDrawRect(&renderer, &rect);

		if (mCollider != nullptr)
		{
			mCollider->Size = Vector2(details.width, details.height);
			mCollider->Render(renderer);
		}
	}
}

void Particle::Update(float deltaTime, const ParticleGenDetails& details)
{
	if (mIsAlive)
	{
		if (mCollider != nullptr)
		{
			mCollider->Update(deltaTime);
		}

		mLifespan += deltaTime;

		if (mLifespan > details.duration)
		{
			mIsAlive = false;
		}
	}
}
