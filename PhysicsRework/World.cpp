#include "World.h"
#include "SDL.h"
#include <SDL_rect.h>

std::vector<SDL_Point> World::DebugPointsToRenderThisFrame = std::vector<SDL_Point>();
Vector2 World::Gravity = Vector2(0.0f, -9.81f);

void World::Setup()
{
	m_Bodies = std::vector<Body*>();

	m_Bodies.push_back(new Body(400, 780, 400, FLT_MAX)); //Floor

	m_Bodies.push_back(new Body(400, 400, 64.0f, 100.0f));
}

void World::Update(float dt)
{












	


	///Integrate Forces
	for (size_t i = 0; i < m_Bodies.size(); i++)
	{
		if (m_Bodies[i]->InvMass == 0.0f)
			continue;

		m_Bodies[i]->Vel += (World::Gravity + m_Bodies[i]->Force * m_Bodies[i]->InvMass) * dt;
		m_Bodies[i]->AngularVel += m_Bodies[i]->Torque * m_Bodies[i]->InvInertia * dt;
	}
}

void World::Render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for (auto itr : m_Bodies)
	{
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawLine(renderer, (int)itr->TL.X, (int)itr->TL.Y, (int)itr->TR.X, (int)itr->TR.Y);
		SDL_RenderDrawLine(renderer, (int)itr->TR.X, (int)itr->TR.Y, (int)itr->BR.X, (int)itr->BR.Y);
		SDL_RenderDrawLine(renderer, (int)itr->BR.X, (int)itr->BR.Y, (int)itr->BL.X, (int)itr->BL.Y);
		SDL_RenderDrawLine(renderer, (int)itr->BL.X, (int)itr->BL.Y, (int)itr->TL.X, (int)itr->TL.Y);
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
			rect.y = itr.y - 2;
			SDL_RenderFillRect(renderer, &rect);
		}
	
		DebugPointsToRenderThisFrame.clear();
	}

	SDL_RenderPresent(renderer);
}