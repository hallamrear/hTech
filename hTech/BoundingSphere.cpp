#include "pch.h"
#include "BoundingSphere.h"
#include "Console.h"
#include "Camera.h"
#include "Transform.h"
#include "rapidjson/rapidjson.h"
#include "imgui.h"

BoundingSphere::BoundingSphere(Transform& transform, float radius, int pointCount)
	: Collider(transform)
{
	mPointCount = pointCount;
	mType = COLLIDER_TYPE::COLLIDER_SPHERE;
	Radius = radius;
}

BoundingSphere::~BoundingSphere()
{
	
}

void BoundingSphere::Update(float DeltaTime)
{
	
}

/// <summary>
/// Sets number of points that represent the circle. If the parameter passed is less than 4, the point count is set to 4.
/// </summary>
/// <param name="pointCount"></param>
void BoundingSphere::SetPointCount(int pointCount)
{
	if (pointCount > 4)
	{
		mPointCount = pointCount;
		return;
	}

	mPointCount = 4;
}

void BoundingSphere::Render(SDL_Renderer& renderer)
{
	if(Console::Query("DrawColliders") != 0)
	{
		//Fibonacci sphere
		float angleIncrement = M_TAU * M_GOLDENRATIO;
		SDL_SetRenderDrawColor(&renderer, 0, 255, 255, 255);
		Vector2 centre = Camera::WorldToScreen(mTransform.Position);
		SDL_RenderDrawPoint(&renderer, (int)centre.X, (int)centre.Y);
		Vector2 point;

		for (int i = 0; i < mPointCount; i++)
		{
			float t = (float)i / mPointCount;
			float angle1 = acos(1 - 2 * t);
			float angle2 = angleIncrement * i;

			//float x = sin(angle1) * cos(angle2);
			//float y = sin(angle1) * sin(angle2);
			//float z = cos(angle1);

			point.X = mTransform.Position.X + Radius * (float)cos(angle1);
			point.Y = mTransform.Position.Y + Radius * (float)sin(angle2);
			point = Camera::WorldToScreen(point);
			SDL_RenderDrawPoint(&renderer, (int)point.X, (int)point.Y);
		}
	}
}

Vector2 BoundingSphere::FindFurthestPoint(Vector2 direction) const
{
	return mTransform.Position + (direction.GetNormalized() * Radius);
}

void BoundingSphere::GetColliderAsPoints(Vector2 points[]) const
{
	//todo : implement
}

void BoundingSphere::Serialize(Serializer& writer) const
{
	writer.String("BoundingSphere_Radius");		writer.Double((double)Radius);
	writer.String("BoundingSphere_PointCount"); writer.Int(mPointCount);
}

void BoundingSphere::Deserialize(SerializedValue& value)
{
	auto radius = value.FindMember("BoundingSphere_Radius");
	if (radius->value.IsDouble())
	{
		Radius = radius->value.GetDouble();
	}

	auto pointCount = value.FindMember("BoundingSphere_PointCount");
	if (pointCount->value.IsInt())
	{
		mPointCount = pointCount->value.GetInt();
	}
}

void BoundingSphere::RenderProperties()
{
	ImGui::SliderFloat("Radius: ", &Radius, 1.0f, 512.0f, "%.0f");
	ImGui::SliderInt("Point Count: ", &mPointCount, 4, 64);
}
