#include "pch.h"
#include "Particle.h"
#include "Camera.h"
#include "Component_ParticleGen.h"
#include "Component_Rigidbody.h"
#include "Collision.h"

Particle::Particle()
{
	srand(time(0));
	mLifespan = 0.0f;
	AddComponent<RigidbodyComponent>();
	RigidbodyComponent* component = GetComponent<RigidbodyComponent>();
	component->SetCollider(COLLIDER_TYPE::COLLIDER_AABB);
	component->GetCollider()->IsOverlap = true;
	Vector2 dir;
	dir.X = rand() % 5000 - 2500;
	dir.Y = rand() % 5000 - 2500;
	component->AddExternalForce(dir);

	GetTransform().Position.X = rand() % 25 - 12;
	GetTransform().Position.Y = rand() % 25 - 12;
}

Particle::~Particle()
{

}

void Particle::Reset()
{
	mLifespan = 0.0f;
	mIsAlive = true;
}

void Particle::Render(SDL_Renderer& renderer, const ParticleGenDetails& details)
{
	if (mIsAlive)
	{
		Vector2 screenPos = Camera::WorldToScreen(GetTransform().Position);
		SDL_Rect rect{ (int)screenPos.X, (int)screenPos.Y, details.width, details.height };
		SDL_SetRenderDrawColor(&renderer, details.colour.R, details.colour.G, details.colour.B, details.colour.A);
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
			//mIsAlive = false;
		}
	}
}
