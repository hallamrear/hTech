#pragma once
#include <vector>
#include "Vector2.h"

class Body;

struct Manifold
{
	std::vector<Vector2> ContactPoints = std::vector<Vector2>();
	bool HasCollided = false;
	float Depth = 0.0f;
	Vector2 Normal;
	Body* BodyA = nullptr;
	Body* BodyB = nullptr;
	int edgeId = -1;

	Manifold() {};
	Manifold(const Manifold& other) = default;
};