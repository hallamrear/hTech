#pragma once

class RigidbodyComponent;

class HTECH_FUNCTION_EXPORT CollisionManifold
{
public:
	bool HasCollided = false;
	float Depth = 0.0f;
	Vector2 Normal;
	std::vector<Vector2> ContactPoints = std::vector<Vector2>();
	RigidbodyComponent* BodyA = nullptr;
	RigidbodyComponent* BodyB = nullptr;
};
