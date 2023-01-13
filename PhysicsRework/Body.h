#pragma once
#include "Vector2.h"
#include <SDL_rect.h>
#include <vector>
#include "Material.h"
#include "MathsUtils.h"
#include "RenderLine.h"

class PhysicsMaterial;

class Body
{
private:
	bool m_IsStatic = false;
	float GetInertiaTensor();
	void CreateEdges();
	std::vector<Edge> m_Edges;
	void CalculateCentreOfMass();
	std::vector<Vector2> m_Vertices;
	std::vector<Vector2> m_TransformedVertices;

public:

	PhysicsMaterial Material;
	Vector2 Pos;
	Vector2 CentreOfMass;
	float	Rot;
	Vector2 Vel;
	float	AngularVel;
	Vector2 Force;
	float	Torque;

	float	Mass;
	float	InvMass;
	float	Inertia;
	float	InvInertia;

	Body(int x, int y, float mass, std::vector<Vector2> vertices, PhysicsMaterial material);
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

