#include "World.h"
#include "SDL.h"
#include <SDL_rect.h>
#include "Collision.h"
#include "Manifold.h"

std::vector<Vector2> World::DebugPointsToRenderThisFrame = std::vector<Vector2>();
std::vector<Line> World::DebugLinesToRenderThisFrame = std::vector<Line>();
Vector2 World::Gravity = Vector2(0.0f, -9.81); //This should be minus but SDL coords make it funky.

Vector2 WorldToScreen(Vector2 worldPosition)
{
	return Vector2(worldPosition.X, 600 - worldPosition.Y);
}

void World::Setup()
{
	m_Manifolds = std::vector<Manifold*>();
	m_Bodies = std::vector<Body*>();

	m_Bodies.push_back(new Body(400, 500, 64.0f, 1000.0f));
	m_Bodies.push_back(new Body(400, 300, 128.0f, FLT_MAX)); //Floor
}

void World::DetermineCollisions()
{
	for (int i = 0; i < (int)m_Bodies.size(); ++i)
	{
		Body* bodyA = m_Bodies[i];

		for (int j = i + 1; j < (int)m_Bodies.size(); ++j)
		{
			Body* bodyB = m_Bodies[j];

			Manifold manifold;

			if (Collision::SeperatingAxisTheory_Original(*bodyA, *bodyB, &manifold))
			{
				Manifold* manifoldptr = new Manifold(manifold);
				m_Manifolds.push_back(manifoldptr);
			}
		}
	}

	return;

}

void World::Update(float dt)
{
	///Clear Existing Manifolds
	for (size_t i = 0; i < m_Manifolds.size(); i++)
	{
		delete m_Manifolds[i];
		m_Manifolds[i] = nullptr;
	}
	m_Manifolds.clear();

	///Generate Collision Manifolds
	DetermineCollisions();

	///Integrate Forces
	for (auto& body : m_Bodies)
	{
		if (body->InvMass == 0.0f)
			continue;

		body->Vel += (World::Gravity + body->Force * body->InvMass) * dt;
		body->AngularVel += body->Torque * body->InvInertia * dt;

		body->Update(dt);
	}

	///Perform Physics Pre-step calculations
	for (auto itr = m_Arbiters.begin(); itr != m_Arbiters.end(); itr++)
	{
		//itr->CalculateImpulses();
	}

	///Perform Physics Iterations
	for (size_t i = 0; i < Iterations; i++)
	{
		for (auto itr = m_Arbiters.begin(); itr != m_Arbiters.end(); itr++)
		{
			//itr->ApplyImpulses();
		}
	}

	///Integrate Velocities.	
	for (auto& body : m_Bodies)
	{
		body->Pos += body->Vel * dt;
		body->Rot += body->AngularVel * dt;

		body->Force = Vector2::Zero;
		body->Torque = 0.0f;
	}
}

void World::Render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for (auto itr : m_Bodies)
	{
		Vector2 tl, tr, br, bl;
		tl = WorldToScreen(itr->TL);
		tr = WorldToScreen(itr->TR);
		br = WorldToScreen(itr->BR);
		bl = WorldToScreen(itr->BL);

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawLine(renderer, (int)tl.X, (int)tl.Y, (int)tr.X, (int)tr.Y);
		SDL_RenderDrawLine(renderer, (int)tr.X, (int)tr.Y, (int)br.X, (int)br.Y);
		SDL_RenderDrawLine(renderer, (int)br.X, (int)br.Y, (int)bl.X, (int)bl.Y);
		SDL_RenderDrawLine(renderer, (int)bl.X, (int)bl.Y, (int)tl.X, (int)tl.Y);
	}


	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect rect;
	rect.w = 4;
	rect.h = 4;
	Vector2 pt;
	for (auto itr : DebugPointsToRenderThisFrame)
	{
		pt = WorldToScreen(itr);
		rect.x = pt.X - 2;
		rect.y = (pt.Y - 2);
		SDL_RenderFillRect(renderer, &rect);
	}
	DebugPointsToRenderThisFrame.clear();

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	Vector2 p1, p2;
	for (auto itr : DebugLinesToRenderThisFrame)
	{
		p1 = WorldToScreen(itr.P1);
		p2 = WorldToScreen(itr.P2);
		SDL_RenderDrawLine(renderer, p1.X, p1.Y, p2.X, p2.Y );
	}
	DebugLinesToRenderThisFrame.clear();


	SDL_RenderPresent(renderer);
}