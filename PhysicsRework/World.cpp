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

void World::Setup()
{
	m_Manifolds = std::vector<Manifold*>();
	Bodies = std::vector<Body*>();

	Bodies.push_back(new Body(400, 490, 64.0f, 1000.0f));
	Bodies.back()->Rot = 45.0f;
	Bodies.back()->Rot = 0.0f;
	Bodies.push_back(new Body(400, 300, 128.0f, FLT_MAX)); //Floor
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
			if (Collision::PrimVsPrim(bodyA, bodyB, manifold))
			{
				SDL_Color green;
				green.r = 0;
				green.g = 255;
				green.b = 0;
				green.a = 255;
				World::DebugLinesToRenderThisFrame.push_back(Line(Point(50.0f, 50.0f), Point(250.0f, 250.0f), green));

				m_Manifolds.push_back(manifold);
				//Vector2 centre = manifold.BodyA_Reference->GetEdge(manifold.BodyAEdgeIndex).GetCentrePoint();
				//World::DebugLinesToRenderThisFrame.push_back(Line(centre, centre + (manifold.Normal * manifold.Depth)));

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

void World::Update(float dt)
{

	///Clear Existing Manifolds
	for (size_t i = 0; i < m_Manifolds.size(); i++)
	{
		m_Manifolds[i]->BodyA_Reference->Pos += Vector2(m_Manifolds[i]->Normal * m_Manifolds[i]->Depth);

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

		//body->Vel += (World::Gravity + body->Force * body->InvMass) * dt;
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
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	for (auto itr : Bodies)
	{
		for (size_t i = 0; i < itr->m_Edges.size(); i++)
		{
			Edge edge = itr->GetEdge(i);
			p1 = WorldToScreen(*edge.A);
			p2 = WorldToScreen(*edge.B);
			SDL_RenderDrawLine(renderer, (int)p1.X, (int)p1.Y, (int)p2.X, (int)p2.Y);
		}
	}

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_Rect rect;
	rect.w = 4;
	rect.h = 4;
	Vector2 pt;
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
		p1 = WorldToScreen(itr.A);
		p2 = WorldToScreen(itr.B);
		SDL_RenderDrawLine(renderer, p1.X, p1.Y, p2.X, p2.Y );
	}
	DebugLinesToRenderThisFrame.clear();

	//for (auto& manifold : m_Manifolds)
	//{
	//	Edge edgeA = manifold->BodyA_Reference->GetEdge(manifold->BodyAEdgeIndex);
	//	Edge edgeB = manifold->BodyB_Incident->GetEdge(manifold->BodyBEdgeIndex);
	//}

	SDL_RenderPresent(renderer);
}