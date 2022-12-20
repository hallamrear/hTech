#pragma once
#include <vector>
#include "Body.h"

struct SDL_Renderer;
struct Manifold;
struct Arbiter;

struct Line
{
	Vector2 P1;
	Vector2 P2;

	Line(Vector2 p1, Vector2 p2) : P1(p1), P2(p2) {};
};

class World
{
private:
	std::vector<Body*> m_Bodies;
	std::vector<Manifold*> m_Manifolds;
	std::vector<Arbiter*> m_Arbiters;
	void DetermineCollisions();

public:
	static std::vector<Vector2> DebugPointsToRenderThisFrame;
	static std::vector<Line>	  DebugLinesToRenderThisFrame;
	static Vector2 Gravity;
	int Iterations = 16;

	void Setup();
	void Render(SDL_Renderer* renderer);
	void Update(float dt);
};

