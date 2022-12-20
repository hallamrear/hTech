#pragma once
#include <vector>
#include "Vector2.h"

struct Manifold
{
	std::vector<Vector2> ContactPoints;
	bool HasCollided = false;
	float Depth = INFINITY;
	Vector2 Normal;
};