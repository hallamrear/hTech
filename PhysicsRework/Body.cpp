#include "Body.h"
#include "MathsUtils.h"
#include "World.h"
#include "Collision.h"


Body::Body(int x, int y, INERTIA_MOMENT inertiaMoment, float mass, std::vector<Vector2> vertices)
{

	srand(NULL);
	Pos.X = x;
	Pos.Y = y;

	Rot = 0.0f;
	Vel = Vector2::Zero;
	AngularVel = 0.0f;
	Force = Vector2::Zero;
	Torque = 0.0f;
	Friction = 5.0f;

	Mass = mass;

	if (mass < FLT_MAX && mass > 0)
	{
		m_IsStatic = false;
		InvMass = 1.0f / (float)Mass;
		Inertia = GetInertiaTensor(inertiaMoment);
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
}

float Body::GetInertiaTensor(INERTIA_MOMENT intertiaMoment)
{
	switch (intertiaMoment)
	{
	case INERTIA_MOMENT::Square:
		break;
	case INERTIA_MOMENT::UniformDistributedWeight:
		break;
	default:
		break;
	}

	return 1.0f;
}

Body::~Body()
{
}

void Body::CreateEdges()
{
	Vector2* v1;
	Vector2* v2;

	int vertexCount = m_Vertices.size();
	for (int i = 0; i < vertexCount - 1; i++)
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
	for (size_t i = 0; i < m_Vertices.size() - 1; i++)
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
	World::DebugLinesToRenderThisFrame.push_back(Line(Pos, Pos + Vel));
	World::DebugLinesToRenderThisFrame.push_back(Line(Pos + Vector2(45.0f, 0.0f), Pos + Vector2(45.0f, 0.0f) + Vector2(0.0f, MathsUtils::ConvertToDegrees(AngularVel))));

	//Set drawing colour if static or not
	SDL_Color color{};
	if (m_IsStatic)
	{
		color.r = 255;
		color.g = 0;
		color.b = 0;
		color.a = 255;
	}
	else
	{
		color.r = 0;
		color.g = 255;
		color.b = 0;
		color.a = 255;
	}


	//Draw shape edges.
	for (int i = 0; i < m_Edges.size(); i++)
	{
		Edge& edge = m_Edges[i];
		Line line = Line(*edge.A, *edge.B);
		World::DebugLinesToRenderThisFrame.push_back(line);
		Vector2 Normal = line.GetNormal();
		line.A = Vector2((line.A.X + line.B.X) / 2.0f, (line.A.Y + line.B.Y) / 2.0f);
		line.B = line.A + (Normal * 15);
		line.colour.r = 255; line.colour.g = 0; line.colour.b = 0;
		World::DebugLinesToRenderThisFrame.push_back(line);
	}
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
