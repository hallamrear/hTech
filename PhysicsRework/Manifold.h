#pragma once
#include <vector>
#include "Vector2.h"

class Body;

struct Manifold
{
	std::vector<Vector2> ContactPoints = std::vector<Vector2>();
	bool HasCollided = false;
	float Depth = INFINITY;
	Vector2 Normal;

	Body* BodyA_Reference = nullptr;
	int   BodyAEdgeIndex = -1;
	Body* BodyB_Incident = nullptr;
	int   BodyBEdgeIndex = -1;

	Manifold() {};
};