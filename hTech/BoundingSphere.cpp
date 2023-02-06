#include "pch.h"
#include "BoundingSphere.h"
#include "Console.h"
#include "Camera.h"
#include "Transform.h"
#include "rapidjson/rapidjson.h"
#include "imgui.h"

BoundingSphere::BoundingSphere(const Transform& transform, float radius, int pointCount)
	: Collider(transform)
{
	m_PointCount = pointCount;
	m_Type = COLLIDER_TYPE::COLLIDER_SPHERE;
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
		m_PointCount = pointCount;
		return;
	}

	m_PointCount = 4;
}

void BoundingSphere::Render(SDL_Renderer& renderer)
{
	if(Console::Query("DrawColliders") != 0)
	{
		//Fibonacci sphere
		float angleIncrement = M_TAU * M_GOLDENRATIO;
		SDL_SetRenderDrawColor(&renderer, 0, 255, 255, 255);
		Vector2 centre = Camera::WorldToScreen(m_EntityTransform.Position);
		SDL_RenderDrawPoint(&renderer, (int)centre.X, (int)centre.Y);
		float angle = 0.0f;
		Vector2 point;
		point.X = m_EntityTransform.Position.X + (Radius * (float)sin(angle));
		point.Y = m_EntityTransform.Position.Y + (Radius * (float)cos(angle));
		point = Camera::WorldToScreen(point);
		Vector2 lastPoint = Vector2();

		for (int i = 1; i < m_PointCount + 1; i++)
		{
			lastPoint = point;
			angle = Utils::Maths::ConvertToRadians((360.0f / m_PointCount) * i);
			point.X = m_EntityTransform.Position.X + (Radius * (float)sin(angle));
			point.Y = m_EntityTransform.Position.Y + (Radius * (float)cos(angle));
			point = Camera::WorldToScreen(point);
			SDL_RenderDrawLine(&renderer, (int)lastPoint.X, (int)lastPoint.Y, (int)point.X, (int)point.Y);
		}
	}
}

Vector2 BoundingSphere::FindFurthestPoint(Vector2 direction) const
{
	return m_EntityTransform.Position + (direction.GetNormalized() * Radius);
}

void BoundingSphere::GetColliderAsPoints(Vector2 points[]) const
{
	//todo : implement
}

void BoundingSphere::Serialize(Serializer& writer) const
{
	writer.String("BoundingSphere_Radius");		writer.Double((double)Radius);
	writer.String("BoundingSphere_PointCount"); writer.Int(m_PointCount);
}

void BoundingSphere::Deserialize(SerializedValue& value)
{
	auto radius = value.FindMember("BoundingSphere_Radius");
	if (radius->value.IsDouble())
	{
		Radius = (float)radius->value.GetDouble();
	}

	auto pointCount = value.FindMember("BoundingSphere_PointCount");
	if (pointCount->value.IsInt())
	{
		m_PointCount = pointCount->value.GetInt();
	}
}

void BoundingSphere::RenderProperties()
{
	ImGui::SliderFloat("Radius: ", &Radius, 1.0f, 512.0f, "%.0f");
	ImGui::SliderInt("Point Count: ", &m_PointCount, 4, 64);
}
