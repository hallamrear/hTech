#pragma once
#include <vector>
#include "Vector2.h"

class Body;

struct Manifold
{
	std::vector<Vector2> ContactPoints = std::vector<Vector2>();
	bool HasCollided = false;
	float Depth = INFINITY;
	const Body* BodyA = nullptr;
	int   BodyAEdgeIndex = -1;
	const Body* BodyB = nullptr;
	int   BodyBEdgeIndex = -1;

	Manifold() {};
};