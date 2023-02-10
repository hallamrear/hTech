#include "World.h"
#include "SDL.h"
#include <SDL_rect.h>
#include "Collision.h"
#include "Manifold.h"
#include "CollisionSolver.h"
#include "Body.h"

SDL_Renderer* World::Renderer = nullptr;
std::vector<Vector2> World::DebugPointsToRenderThisFrame = std::vector<Vector2>();
std::vector<Line> World::DebugLinesToRenderThisFrame = std::vector<Line>();
Vector2 World::Gravity = Vector2(0.0f, 0.0f -9.0f);

void World::Setup()
{
	FloorVertices = std::vector<Vector2>();
	FloorVertices.push_back(Vector2(-500.0f, 32.0f));
	FloorVertices.push_back(Vector2( 500.0f, 32.0f));
	FloorVertices.push_back(Vector2( 500.0f, -32.0f));
	FloorVertices.push_back(Vector2(-500.0f, -32.0f));

	//TL->TR->BR->BL
	SquareVertices	 = std::vector<Vector2>(); 
	SquareVertices.push_back(Vector2(-32.0f,  32.0f));
	SquareVertices.push_back(Vector2( 32.0f,  32.0f));
	SquareVertices.push_back(Vector2( 32.0f, -32.0f));
	SquareVertices.push_back(Vector2(-32.0f, -32.0f));

	TriangleVertices = std::vector<Vector2>();
	TriangleVertices.push_back(Vector2(-64.0f, -32.0f));
	TriangleVertices.push_back(Vector2(0.0f, 32.0f));
	TriangleVertices.push_back(Vector2(64.0f, -32.0f));

	PentagonVertices = std::vector<Vector2>();
	PentagonVertices.push_back(Vector2(0.0f, -100.0f));
	PentagonVertices.push_back(Vector2(-95.0f, -31.0f));
	PentagonVertices.push_back(Vector2(-59.0f, 81.0f));
	PentagonVertices.push_back(Vector2(59.0f, 81.0f));
	PentagonVertices.push_back(Vector2(95.0f, -31.0f));

	m_Collisions = std::vector<CollisionSolver*>();
	m_Manifolds = std::vector<Manifold>();
	Bodies = std::vector<Body*>();

	Bodies.push_back(new Body(0, 0, 10.0f, PentagonVertices, PhysicsMaterial(0.3f, 0.95f)));
	Bodies.push_back(new Body(-100,   0,  100.0f, SquareVertices, PhysicsMaterial(0.6f, 0.1f)));
	Bodies.push_back(new Body(   0,-300, FLT_MAX, FloorVertices,  PhysicsMaterial(1.2f, 0.05f)));
	Bodies.push_back(new Body(   0, 300, FLT_MAX, FloorVertices,  PhysicsMaterial(1.2f, 0.05f)));
	Bodies.push_back(new Body(-400,   0, FLT_MAX, FloorVertices,  PhysicsMaterial(1.2f, 0.05f)));	Bodies.back()->Rot = 90.0f;		   
	Bodies.push_back(new Body( 400,   0, FLT_MAX, FloorVertices,  PhysicsMaterial(1.2f, 0.05f)));	Bodies.back()->Rot = 90.0f;		  
	Bodies.push_back(new Body( 100,   0,  100.0f, SquareVertices, PhysicsMaterial(0.3f, 0.8f)));
	Bodies.push_back(new Body(100, -250, 100.0f, TriangleVertices, PhysicsMaterial(1.2f, 0.05f)));

}

void World::DetermineCollisions()
{
	for (int i = 0; i < (int)Bodies.size(); ++i)
	{
		Body* bodyA = Bodies[i];

		for (int j = i + 1; j < (int)Bodies.size(); ++j)
		{
			Body* bodyB = Bodies[j];

			if (bodyA == bodyB)
				continue;

			if (bodyA->IsStatic() && bodyB->IsStatic())
				continue;

			Manifold manifold;

			if (Collision::PolygonVsPolygon(bodyA, bodyB, manifold))
			{
				m_Collisions.push_back(new GJKCollisionSolver(manifold));
			}
		}
	}
}

void World::Update(float dt)
{
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);

	///Integrate Forces
	for (auto& body : Bodies)
	{
		body->Update(dt);

		if (body->IsStatic())
			continue;

		{
			body->Vel += Gravity + (Vector2(body->Force * body->InvMass)) * dt;
			body->AngularVel += body->Torque * body->InvInertia * dt;
		}
	}


	///Generate Collision Manifolds
	DetermineCollisions();

	///Perform Physics Pre-step calculations
	for (size_t i = 0; i < m_Collisions.size(); i++)
	{
		m_Collisions[i]->Prestep();
	}

	///Perform Physics Iterations
	for (int i = 0; i <= Iterations; i++)
	{
		for (size_t i = 0; i < m_Collisions.size(); i++)
		{
			m_Collisions[i]->PhysicsStep();
		}
	}
	
	///Integrate Velocities.	
	for (auto& body : Bodies)
	{
		if (body->IsStatic())
			continue;

		body->Pos += (body->Vel * dt);
		body->Rot += MathsUtils::ConvertToDegrees(body->AngularVel) * dt;

		body->Force = Vector2::Zero;
		body->Torque = 0.0f;
	}

}

void World::Render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	Vector2 centre = WorldToScreen(Vector2());
	SDL_RenderDrawPoint(renderer, centre.X, centre.Y);

	Vector2 p1, p2;
	Vector2 pt;

	for (auto& body : Bodies)
	{
		body->Render();
	}

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect rect{};
	rect.w = 4;
	rect.h = 4;
	for (auto& itr : DebugPointsToRenderThisFrame)
	{
		pt = WorldToScreen(itr);
		rect.x = (int)(pt.X - 2.0f);
		rect.y = (int)(pt.Y - 2.0f);
		SDL_RenderFillRect(renderer, &rect);
	}
	DebugPointsToRenderThisFrame.clear();

	for (auto& itr : DebugLinesToRenderThisFrame)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		DrawLineToScreen(renderer, itr.A, itr.B);
	}
	DebugLinesToRenderThisFrame.clear();

	///Clear Existing solvers
	for (size_t i = 0; i < m_Collisions.size(); i++)
	{
		delete m_Collisions[i];
		m_Collisions[i] = nullptr;
	}

	m_Collisions.clear();

	SDL_RenderPresent(renderer);
}