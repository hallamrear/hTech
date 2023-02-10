#pragma once
#include <vector>
#include "Body.h"
#include "Manifold.h"

struct SDL_Renderer;
struct CollisionSolver;

class World
{
private:
	std::vector<Vector2> SquareVertices;
	std::vector<Vector2> TriangleVertices;
	std::vector<Vector2> PentagonVertices;
	std::vector<Vector2> FloorVertices;

	std::vector<Manifold> m_Manifolds;
	std::vector<CollisionSolver*>  m_Collisions;
	void DetermineCollisions();

public:
	static SDL_Renderer* Renderer;
	std::vector<Body*> Bodies;
	static std::vector<Vector2> DebugPointsToRenderThisFrame;
	static std::vector<Line>	DebugLinesToRenderThisFrame;
	static Vector2 Gravity;
	int Iterations = 32;

	void Setup();
	void Render(SDL_Renderer* renderer);
	void Update(float dt);
};

