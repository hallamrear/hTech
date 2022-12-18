#pragma once
#include <vector>
#include "Body.h"

struct SDL_Renderer;

struct World
{
	static std::vector<SDL_Point> DebugPointsToRenderThisFrame;

	std::vector<Body*> m_Bodies;

	void Setup();
	void Render(SDL_Renderer* renderer);
	void Update(float dt);
};

