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

void DrawLineToScreen(SDL_Renderer* renderer, Point pointA, Point pointB)
{
	Vector2 p1 = pointA, p2 = pointB;
	p1 = WorldToScreen(p1);
	p2 = WorldToScreen(p2);
	SDL_RenderDrawLine(renderer, p1.X, p1.Y, p2.X, p2.Y);
}

void World::Setup()
{
	m_Manifolds = std::vector<Manifold*>();
	Bodies = std::vector<Body*>();

	Bodies.push_back(new Body(400, 490, 64.0f, 1000.0f));
	Bodies.back()->Rot = 45.0f;
-	Bodies.push_back(new Body(400, 300, 128.0f, FLT_MAX)); //Floor
}

void World::DetermineCollisions()
{
	for (int i = 0; i < (int)Bodies.size(); ++i)
	{
		Body* bodyA = Bodies[i];

		for (int j = i + 1; j < (int)Bodies.size(); ++j)
		{
			Body* bodyB = Bodies[j];

			Manifold* manifold = new Manifold();
			if (Collision::PolygonVsPolygon(*bodyA, *bodyB, *manifold))
			{
				SDL_Color green;
				green.r = 0;
				green.g = 255;
				green.b = 0;
				green.a = 255;
				World::DebugLinesToRenderThisFrame.push_back(Line(Point(50.0f, 50.0f), Point(250.0f, 250.0f), green));

				m_Manifolds.push_back(manifold);
			}
			else
			{
				delete manifold;
				manifold = nullptr;

				SDL_Color red;
				red.r = 255;
				red.g = 0;
				red.b = 0;
				red.a = 0;
				World::DebugLinesToRenderThisFrame.push_back(Line(Point(50.0f, 50.0f), Point(250.0f, 250.0f), red));
			}
		}
	}
}

static int mX;
static int mY;
static Vector2 mPos;

void World::Update(float dt)
{
	SDL_GetMouseState(&mX, &mY);
	mPos = Vector2(mX, 600 + mY);
	World::DebugPointsToRenderThisFrame.push_back(mPos);

	Body* b;
	///Clear Existing Manifolds
	for (size_t i = 0; i < m_Manifolds.size(); i++)
	{
		b = const_cast<Body*>(m_Manifolds[i]->BodyA);
		b->Pos += Vector2(m_Manifolds[i]->Normal * m_Manifolds[i]->Depth);

		delete m_Manifolds[i];
		m_Manifolds[i] = nullptr;
	}
	m_Manifolds.clear();

	///Generate Collision Manifolds
	DetermineCollisions();

	///Integrate Forces
	for (auto& body : Bodies)
	{
		body->Update(dt);

		if (body->InvMass == 0.0f)
			continue;

		body->Vel += (World::Gravity + body->Force * body->InvMass) * dt;
		body->AngularVel += body->Torque * body->InvInertia * dt;
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
	for (auto& body : Bodies)
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

	Vector2 p1, p2;
	Vector2 pt;
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	for (auto itr : Bodies)
	{
		for (size_t i = 0; i < itr->m_Edges.size(); i++)
		{
			DebugLinesToRenderThisFrame.push_back(itr->m_Edges[i]->GetLine());
		}
	}


	//Line test = Line(Vector2(50.0f, 50.0f), Vector2(250.0f, 250.0f));
	//DebugLinesToRenderThisFrame.push_back(test);
	//auto r = MathsUtils::FindClosestPointOnLine(WorldToScreen(Vector2(mX, mY)), test.A, test.B);
	//Vector2 point = r.ClosestPoint;
	//DebugPointsToRenderThisFrame.push_back(point);


	for (auto& manifold : m_Manifolds)
	{
		for (size_t i = 0; i < manifold->ContactPoints.size(); i++)
		{
			SDL_Color color;
			color.r = 255; color.g = 255; color.b = 255; color.a = 255;
			Line line = Line(manifold->ContactPoints[i], manifold->ContactPoints[i] + (manifold->Normal * manifold->Depth), color);
			DebugLinesToRenderThisFrame.push_back(line);
			DebugPointsToRenderThisFrame.push_back(manifold->ContactPoints[i]);
		}
	}

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect rect;
	rect.w = 4;
	rect.h = 4;
	for (auto& itr : DebugPointsToRenderThisFrame)
	{
		pt = WorldToScreen(itr);
		rect.x = pt.X - 2;
		rect.y = (pt.Y - 2);
		SDL_RenderFillRect(renderer, &rect);
	}
	DebugPointsToRenderThisFrame.clear();

	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	for (auto& itr : DebugLinesToRenderThisFrame)
	{
		SDL_SetRenderDrawColor(renderer, itr.colour.r, itr.colour.g, itr.colour.b, 255);
		DrawLineToScreen(renderer, itr.A, itr.B);
	}
	DebugLinesToRenderThisFrame.clear();

	SDL_RenderPresent(renderer);
}