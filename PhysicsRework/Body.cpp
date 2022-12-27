#include "Body.h"
#include "MathsUtils.h"
#include "World.h"
#include "Collision.h"

Body::Body(int x, int y, int size, float mass)
{
	srand(NULL);
	Size = size;
	Pos.X = x;
	Pos.Y = y;

	Rot = 0.0f;
	Vel = Vector2::Zero;
	AngularVel = 0.0f;
	Force = Vector2::Zero;
	Torque = 0.0f;
	Friction = 0.0f;

	Mass = mass;

	if (mass < FLT_MAX)
	{
		InvMass = 1.0f / Mass;
		Inertia = Mass * (Size * Size + Size * Size) / 12.0f;
		InvInertia = 1.0f / Inertia;
	}
	else
	{
		InvMass = 0.0f;
		Inertia = FLT_MAX;
		InvMass = 0.0f;
	}
	m_Vertices = std::vector<Vector2>();
	m_Vertices.push_back(Vector2());
	m_Vertices.push_back(Vector2());
	m_Vertices.push_back(Vector2());
	m_Vertices.push_back(Vector2());
	CalculateOrientedPositions();

	TL = &m_Vertices[0];
	BL = &m_Vertices[1];
	TR = &m_Vertices[2];
	BR = &m_Vertices[3];

	m_Edges = std::vector<Edge*>();
	m_Edges.push_back(new Edge(*TL, *TR));
	m_Edges.push_back(new Edge(*TR, *BR));
	m_Edges.push_back(new Edge(*BR, *BL));
	m_Edges.push_back(new Edge(*BL, *TL));

}

Body::~Body()
{
}

const Edge& Body::GetEdge(int index) const
{
	return *m_Edges[index];
}

void Body::CalculateOrientedPositions()
{
	//Calculate new point rotations;
	//if (Rot > 0.0f && Rot < 360.0f)
	{
		float rotation = 360.0f - Rot;
		m_Vertices[0] = MathsUtils::RotatePointAroundOriginDegrees(Vector2(Pos.X - (Size / 2), Pos.Y + (Size / 2)), Rot, Pos);
		m_Vertices[1] = MathsUtils::RotatePointAroundOriginDegrees(Vector2(Pos.X - (Size / 2), Pos.Y - (Size / 2)), Rot, Pos);
		m_Vertices[2] = MathsUtils::RotatePointAroundOriginDegrees(Vector2(Pos.X + (Size / 2), Pos.Y + (Size / 2)), Rot, Pos);
		m_Vertices[3] = MathsUtils::RotatePointAroundOriginDegrees(Vector2(Pos.X + (Size / 2), Pos.Y - (Size / 2)), Rot, Pos);
	}
}

void Body::Update(float dt)
{
	srand(NULL);
	//Rot += (rand() % 100) * dt * 5;
	CalculateOrientedPositions();

	for (int i = 0; i < m_Edges.size(); i++)
	{
		Edge* edge = m_Edges[i];
		Line line = Line(*edge->A, *edge->B);
		Vector2 normal = line.GetNormal();
		line.A = Vector2((line.A.X + line.B.X) / 2.0f, (line.A.Y + line.B.Y) / 2.0f);
		line.B = line.A + (normal * 15);
		line.colour.r = 0; line.colour.g = 0; line.colour.b = 0;

		switch (i)
		{
		case 0:
		{
			line.colour.r = 255;
		}
		break;
		case 1:
		{
			line.colour.g = 255;
		}
		break;
		case 2:
		{
			line.colour.b = 255;
		}
		break;
		case 3:
		{
			line.colour.r = 255;
			line.colour.b = 255;
		}
		break;
		}

		World::DebugLinesToRenderThisFrame.push_back(line);
	}
	World::DebugPointsToRenderThisFrame.push_back(Pos);
}

const Vector2& Body::GetSupportVertex(const Vector2& direction) const
{
	float furthestDistance = -100000000.0f;
	int furthestVertexIndex = -1;
	float projectedDistance = -100000000.0f;

	for (size_t i = 0; i < m_Vertices.size(); i++)
	{
		Vector2 vertex = m_Vertices[i];

		projectedDistance = MathsUtils::Dot(vertex, direction);

		if (projectedDistance > furthestDistance)
		{
			furthestDistance = projectedDistance;
			furthestVertexIndex = i;
		}
	}

	return m_Vertices[furthestVertexIndex];
}
