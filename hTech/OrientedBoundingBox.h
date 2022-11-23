#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include "BoundingBox.h"

class HTECH_FUNCTION_EXPORT OrientedBoundingBox : public BoundingBox
{
protected:
	void CalculateRotations();

public:
	Vector2 TopRight;
	Vector2 BottomLeft;

	OrientedBoundingBox(Transform& transform, float size_x, float size_y);
	~OrientedBoundingBox();

	virtual void Update(float DeltaTime);
	virtual void Render(SDL_Renderer& renderer);

	//Fills array with points of the box.
	void GetColliderAsPoints(Vector2 points[]) const;
};
