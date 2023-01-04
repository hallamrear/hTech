#pragma once
#include "Vector2.h"
#include <SDL_rect.h>
#include <vector>
#include "Material.h"
#include "MathsUtils.h"

struct Edge
{
	Point* A;
	Point* B;

	Edge(Point& a, Point& b) : A(&a), B(&b)
	{

	};

	Vector2 GetNormal() const
	{
		float dx = B->X - A->X;
		float dy = B->Y - A->Y;
		return Vector2(-dy, dx).GetNormalized();
	}

	Vector2 GetCentrePoint() const
	{
		return Vector2((A->X + B->X) / 2.0f, (A->Y + B->Y) / 2.0f);
	}

	Line GetLine() const
	{
		return Line(*A, *B);
	}

	Edge& operator=(const Edge& edge)
	{
		A = edge.A;
		B = edge.B;
		return *this;
	};
};

enum class INERTIA_MOMENT
{
	Square = 0,
	UniformDistributedWeight,
};

class Material;

struct Body
{
private:
	bool m_IsStatic = false;
	void CreateEdges();
	float GetInertiaTensor(INERTIA_MOMENT inertiaMoment);

public:
	std::vector<Edge> m_Edges;
	std::vector<Vector2> m_Vertices;
	std::vector<Vector2> m_TransformedVertices;

	bool COLLIDE = false;

	Material material;
	Vector2 Pos;
	float	Rot;
	Vector2 Vel;
	float	AngularVel;
	Vector2 Force;
	float	Torque;
	float	Friction;

	float	Mass;
	float	InvMass;
	float	Inertia;
	float	InvInertia;

	Body(int x, int y, INERTIA_MOMENT inertiaMoment, float mass, std::vector<Vector2> vertices, Material material);
	~Body();

	const Edge& GetEdge(int index) const;
	const Vector2& GetSupportVertex(const Vector2& direction) const;

	void CalculateOrientedPositions();
	void Update(float dt);
	void Render();

	inline bool IsStatic() const
	{
		return m_IsStatic;
	};
};

