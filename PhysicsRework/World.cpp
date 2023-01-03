#include "World.h"
#include "SDL.h"
#include <SDL_rect.h>
#include "Collision.h"
#include "Manifold.h"
#include "CollisionSolver.h"

std::vector<Vector2> World::DebugPointsToRenderThisFrame = std::vector<Vector2>();
std::vector<Line> World::DebugLinesToRenderThisFrame = std::vector<Line>();
Vector2 World::Gravity = Vector2(0.0f, 0.0f - 9.81f);
bool World::TestMode = false;

Vector2 WorldToScreen(Vector2 worldPosition)
{
	return Vector2(worldPosition.X, 600 - worldPosition.Y);
}

void DrawLineToScreen(SDL_Renderer* renderer, Point pointA, Point pointB)
{
	Vector2 p1 = pointA, p2 = pointB;
	p1 = WorldToScreen(p1);
	p2 = WorldToScreen(p2);
	SDL_RenderDrawLine(renderer, (int)p1.X, (int)p1.Y, (int)p2.X, (int)p2.Y);
}

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

	Bodies.push_back(new Body(600, 490, INERTIA_MOMENT::Square, 100.0f, SquareVertices, Material(1.2f, 0.05f)));

	Bodies.push_back(new Body(0, 300, INERTIA_MOMENT::Square, FLT_MAX, FloorVertices, Material(0.0f, 0.4f)));
	Bodies.back()->Rot = 90.0f;
	Bodies.push_back(new Body(800, 300, INERTIA_MOMENT::Square, FLT_MAX, FloorVertices, Material(0.0f, 0.4f)));
	Bodies.back()->Rot = 90.0f;
	Bodies.push_back(new Body(400, 0, INERTIA_MOMENT::Square, FLT_MAX, FloorVertices, Material(0.0f, 0.4f)));
	Bodies.push_back(new Body(400, 600, INERTIA_MOMENT::Square, FLT_MAX, FloorVertices, Material(0.0f, 0.4f)));

	Bodies.push_back(new Body(200, 10, INERTIA_MOMENT::Square, 10.0f, PentagonVertices, Material(0.3f, 0.95f)));
	Bodies.push_back(new Body(400, 90, INERTIA_MOMENT::Square, 100.0f, TriangleVertices, Material(0.3f, 0.95f)));
}

void World::DetermineCollisions()
{
	for (int i = 0; i < (int)Bodies.size(); ++i)
	{
		Body* bodyA = Bodies[i];

		for (int j = i + 1; j < (int)Bodies.size(); ++j)
		{
			Body* bodyB = Bodies[j];

			if (bodyA->IsStatic() && bodyB->IsStatic())
				continue;

			Manifold manifold;			

			if (Collision::PolygonVsPolygon(bodyA, bodyB, manifold))
			{
				m_Manifolds.push_back(manifold);

				m_Collisions.push_back(new AlternateCollisionSolver(manifold));

				//if (bodyA->IsStatic() || bodyB->IsStatic())
				//{
				//	m_Collisions.push_back(new StaticVsDynamicCollisionSolver(manifold));
				//}
				//else
				//{
				//	m_Collisions.push_back(new DynamicVsDynamicCollisionSolver(manifold));
				//}
			}
		}
	}
}

static int mX;
static int mY;
static Vector2 mPos;

void World::Update(float dt)
{
	float inverseDeltaTime = 1.0f / dt;

	SDL_GetMouseState(&mX, &mY);
	mPos = Vector2((float)mX, 600.0f + (float)mY);
	World::DebugPointsToRenderThisFrame.push_back(mPos);

	///Generate Collision Manifolds
	DetermineCollisions();

	///Integrate Forces
	for (auto& body : Bodies)
	{
		body->Update(dt);

		if (TestMode == false)
		{
			if (body->IsStatic() == false)
			{
				//body->Vel += Gravity + (Vector2(body->Force * body->InvMass)) * dt;
				body->Vel += (Vector2(body->Force * body->InvMass)) * dt;
				body->AngularVel += body->Torque * body->InvInertia * dt;
			}
			else
			{
				body->Vel += Vector2(body->Force * body->InvMass) * dt;
			}
		}
		else
		{
			body->Vel += Vector2(body->Force * body->InvMass) * dt;
		}
	}

	if (TestMode == false)
	{
		///Perform Physics Pre-step calculations
		for (size_t i = 0; i < m_Collisions.size(); i++)
		{
			m_Collisions[i]->Prestep(inverseDeltaTime);
		}

		///Perform Physics Iterations
		for (size_t i = 0; i <= Iterations; i++)
		{
			for (size_t i = 0; i < m_Collisions.size(); i++)
			{
				m_Collisions[i]->PhysicsStep();
			}
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

void World::CreateBody()
{
	Bodies.push_back(new Body(600, 490, INERTIA_MOMENT::Square, 100.0f, SquareVertices, Material(0.3f, 0.8f)));
}

void World::Render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	Vector2 p1, p2;
	Vector2 pt;

	for (auto& body : Bodies)
	{
		body->Render();
	}

	for (auto& manifold : m_Manifolds)
	{
		for (size_t i = 0; i < manifold.ContactPoints.size(); i++)
		{
			SDL_Color color{};
			color.r = 255; color.g = 255; color.b = 255; color.a = 255;
			Line line = Line(manifold.ContactPoints[i], manifold.ContactPoints[i] + (manifold.Normal * manifold.Depth), color);
			DebugLinesToRenderThisFrame.push_back(line);
			DebugPointsToRenderThisFrame.push_back(manifold.ContactPoints[i]);
		}
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
		SDL_SetRenderDrawColor(renderer, itr.colour.r, itr.colour.g, itr.colour.b, 255);
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
	///Clear Existing Manifolds
	m_Manifolds.clear();

	SDL_RenderPresent(renderer);
}