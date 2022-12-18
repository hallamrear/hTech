#include "World.h"
#include "SDL.h"
#include <SDL_rect.h>

std::vector<SDL_Point> World::DebugPointsToRenderThisFrame = std::vector<SDL_Point>();
Vector2 World::Gravity = Vector2(0.0f, -9.81f); //This should be minus but SDL coords make it funky.

void World::Setup()
{
	m_Bodies = std::vector<Body*>();

	m_Bodies.push_back(new Body(400, -150, 400, FLT_MAX)); //Floor

	m_Bodies.push_back(new Body(400, 400, 64.0f, 1000.0f));
}

void World::Update(float dt)
{
	//Generate Collision Manifolds

	///Integrate Forces
	for (auto& body : m_Bodies)
	{
		if (body->InvMass == 0.0f)
			continue;

		body->Vel += (World::Gravity + body->Force * body->InvMass) * dt;
		body->AngularVel += body->Torque * body->InvInertia * dt;

		body->CalculateRotatedCorners();
	}




	//Perform Physics Iterations







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
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawLine(renderer, (int)itr->TL.X, 600 - (int)itr->TL.Y, (int)itr->TR.X, 600 - (int)itr->TR.Y);
		SDL_RenderDrawLine(renderer, (int)itr->TR.X, 600 - (int)itr->TR.Y, (int)itr->BR.X, 600 - (int)itr->BR.Y);
		SDL_RenderDrawLine(renderer, (int)itr->BR.X, 600 - (int)itr->BR.Y, (int)itr->BL.X, 600 - (int)itr->BL.Y);
		SDL_RenderDrawLine(renderer, (int)itr->BL.X, 600 - (int)itr->BL.Y, (int)itr->TL.X, 600 - (int)itr->TL.Y);
	}

	if (DebugPointsToRenderThisFrame.size() > 0)
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_Rect rect;
		rect.w = 4;
		rect.h = 4;
		for (auto itr : DebugPointsToRenderThisFrame)
		{
			rect.x = itr.x - 2;
			rect.y = 600 - (itr.y - 2);
			SDL_RenderFillRect(renderer, &rect);
		}
	
		DebugPointsToRenderThisFrame.clear();
	}

	SDL_RenderPresent(renderer);
}