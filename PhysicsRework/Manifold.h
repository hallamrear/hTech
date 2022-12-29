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
	const Body* BodyA = nullptr;
	const Body* BodyB = nullptr;

	Manifold() {};
};