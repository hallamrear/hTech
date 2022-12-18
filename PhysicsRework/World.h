#pragma once
#include <vector>
#include "Body.h"

struct SDL_Renderer;

class World
{
private:
	std::vector<Body*> m_Bodies;

public:
	static std::vector<SDL_Point> DebugPointsToRenderThisFrame;


	void Setup();
	void Render(SDL_Renderer* renderer);
	void Update(float dt);
};

