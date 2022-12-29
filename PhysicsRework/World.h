#pragma once
#include <vector>
#include "Body.h"

struct SDL_Renderer;
struct Manifold;
struct Arbiter;

class World
{
private:
	std::vector<Manifold*> m_Manifolds;
	std::vector<Arbiter*>  m_Arbiters;
	void DetermineCollisions();

public:
	std::vector<Body*> Bodies;
	static std::vector<Vector2> DebugPointsToRenderThisFrame;
	static std::vector<Line>	DebugLinesToRenderThisFrame;
	static Vector2 Gravity;
	int Iterations = 16;

	void Setup();
	void Render(SDL_Renderer* renderer);
	void Update(float dt);
};

