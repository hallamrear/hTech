#include "Body.h"
#include "World.h"
#include "Collision.h"

Body::Body(int x, int y, float mass, std::vector<Vector2> vertices, PhysicsMaterial material)
{
	Material = material;

	srand(NULL);
	Pos.X = x;
	Pos.Y = y;

	Rot = 0.0f;
	Vel = Vector2::Zero;
	AngularVel = 0.0f;
	Force = Vector2::Zero;
	Torque = 0.0f;

	Mass = mass;

	if (mass < FLT_MAX && mass > 0)
	{
		m_IsStatic = false;
		InvMass = 1.0f / (float)Mass;
		Inertia = GetInertiaTensor();
		InvInertia = 1.0f / Inertia;
	}
	else
	{
		m_IsStatic = true;
		InvMass = 0.0f;
		Inertia = FLT_MAX;
		InvMass = 0.0f;
	}

	m_Vertices = vertices;
	m_TransformedVertices = vertices;
	m_Edges = std::vector<Edge>();
	CreateEdges();
	CalculateOrientedPositions();
	CalculateCentreOfMass();
}

void Body::CalculateCentreOfMass()
{
	CentreOfMass = Vector2::Zero;

	for (size_t i = 0; i < m_Vertices.size(); i++)
	{
		CentreOfMass += m_Vertices[i];
	}

	CentreOfMass = CentreOfMass / (float)m_Vertices.size();
}

float Body::GetInertiaTensor()
{
	float w = 5.0f;
	float h = 5.0f;
	return (1.0f / 12.0f) * Mass* (powf(w, 2.0f) + powf(h, 2.0f));
}

Body::~Body()
{

}

void Body::CreateEdges()
{
	m_Edges.clear();

	Vector2* v1;
	Vector2* v2;

	int vertexCount = m_Vertices.size();
	for (int i = 0; i < vertexCount; i++)
	{ 
		v1 = &m_TransformedVertices[i];
		v2 = &m_TransformedVertices[(i + 1) % vertexCount];

		m_Edges.push_back(Edge(*v1, *v2));
	}
}

const Edge& Body::GetEdge(int index) const
{
	return m_Edges[index];
}

void Body::CalculateOrientedPositions()
{
	//Calculate new point rotations;
	for (size_t i = 0; i < m_Vertices.size(); i++)
	{
		m_TransformedVertices[i] = MathsUtils::RotatePointAroundOriginDegrees(Pos + m_Vertices[i], Rot, Pos);
	}
}

void Body::Update(float dt)
{
	srand(NULL);
	//Rot += (rand() % 100) * dt * 5;
	CalculateOrientedPositions();
}

void Body::Render()
{
	//Draw position
	World::DebugPointsToRenderThisFrame.push_back(Pos);

	//Draw velocity and rotational velocity.
	if (!m_IsStatic)
	{
		World::DebugLinesToRenderThisFrame.push_back(Line(Pos, Pos + Vel));
		World::DebugLinesToRenderThisFrame.push_back(Line(Pos + Vector2(45.0f, 0.0f), Pos + Vector2(45.0f, 0.0f) + Vector2(0.0f, MathsUtils::ConvertToDegrees(AngularVel))));
	}

	//Draw shape edges.
	for (int i = 0; i < m_Edges.size(); i++)
	{
		Edge& edge = m_Edges[i];

		SDL_SetRenderDrawColor(World::Renderer, 0, 255, 0, 255);

		DrawLineToScreen(World::Renderer, edge.A, edge.B);
		Vector2 Normal = edge.GetNormal();

		Vector2 A = Vector2((edge.A.X + edge.B.X) / 2.0f, (edge.A.Y + edge.B.Y) / 2.0f);
		Vector2 B = A + (Normal * 15);
		DrawLineToScreen(World::Renderer, A, B);
	}
}

const Vector2& Body::GetSupportVertex(const Vector2& direction) const
{
	Vector2 vertex = m_TransformedVertices[0];
	float furthestDistance = MathsUtils::Dot(vertex, direction);
	int furthestVertexIndex = 0;
	float projectedDistance = -10000.0f;

	for (size_t i = 0; i < m_TransformedVertices.size(); i++)
	{
		vertex = m_TransformedVertices[i];

		projectedDistance = MathsUtils::Dot(vertex, direction);

		if (projectedDistance >= furthestDistance)
		{
			furthestDistance = projectedDistance;
			furthestVertexIndex = i;
		}
	}

	return m_TransformedVertices[furthestVertexIndex];
}
