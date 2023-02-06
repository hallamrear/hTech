#include "pch.h"
#include "BoundingBox.h"
#include "Transform.h"
#include "Camera.h"
#include "imgui.h"
#include "rapidjson/rapidjson.h"
#include "Console.h"

BoundingBox::BoundingBox(const Transform& transform, float size_x, float size_y)
	: Collider(transform)
{
	m_Type = COLLIDER_TYPE::COLLIDER_AABB;

	m_Size.X = size_x;
	m_Size.Y = size_y;
	
	m_TopLeft = Vector2(m_EntityTransform.Position.X - (m_Size.X / 2.0f), m_EntityTransform.Position.Y + (m_Size.Y / 2.0f));
	m_BottomRight = Vector2(m_EntityTransform.Position.X + (m_Size.X / 2.0f), m_EntityTransform.Position.Y - (m_Size.Y / 2.0f));
};

const Vector2 BoundingBox::GetTopLeft() const
{
	return m_TopLeft;
}

const Vector2 BoundingBox::GetBottomRight() const
{
	return m_BottomRight;
}

const Vector2 BoundingBox::GetSize() const
{
	return Vector2(m_BottomRight.X - m_TopLeft.X, m_TopLeft.Y - m_BottomRight.Y);
}

BoundingBox::~BoundingBox()
{
	
};

void BoundingBox::Update(float DeltaTime)
{
	m_TopLeft = Vector2(m_EntityTransform.Position.X - (m_Size.X / 2), m_EntityTransform.Position.Y + (m_Size.Y / 2));
	m_BottomRight = Vector2(m_EntityTransform.Position.X + (m_Size.X / 2), m_EntityTransform.Position.Y - (m_Size.Y / 2));
}

void BoundingBox::Render(SDL_Renderer& renderer)
{
	if (Console::Query("DrawColliders") != 0)
	{
		SDL_Rect r{};
		Vector2 Position = Camera::WorldToScreen(Vector2(m_EntityTransform.Position.X - (m_Size.X / 2), m_EntityTransform.Position.Y + (m_Size.Y / 2)));
		r.x = (int)Position.X;
		r.y = (int)Position.Y;
		r.w = (int)(m_Size.X);
		r.h = (int)(m_Size.Y);

		SDL_SetRenderDrawColor(&renderer, 0, 255, 255, 255);
		SDL_RenderDrawRect(&renderer, &r);
	}
}

void BoundingBox::GetColliderAsPoints(Vector2 points[]) const
{
	points[0] = Vector2(m_TopLeft.X, m_BottomRight.Y); //BottomLeft
	points[1] = m_BottomRight;
	points[2] = Vector2(m_BottomRight.X, m_TopLeft.Y); //TopRight
	points[3] = m_TopLeft;
}

Vector2 BoundingBox::FindFurthestPoint(Vector2 direction) const
{
	Vector2 maxPoint;
	float maxDistance = -FLT_MAX;

	Vector2 vertices[4];
	GetColliderAsPoints(vertices);

	for (int i = 0; i < 4; i++)
	{
		float distance = vertices[i].Dot(direction);
		if (distance > maxDistance)
		{
			maxDistance = distance;
			maxPoint = vertices[i];
		}
	}

	return maxPoint;
}

void BoundingBox::Serialize(Serializer& writer) const
{
	Collider::Serialize(writer);

	writer.String("BoundingBoxSize");
	writer.StartObject();
	writer.String("X"); writer.Double((double)m_Size.X);
	writer.String("Y"); writer.Double((double)m_Size.Y);
	writer.EndObject();
}

void BoundingBox::Deserialize(SerializedValue& value)
{
	Collider::Deserialize(value);

	auto propertiesMember = value.FindMember("Physics Properties");
	if (propertiesMember->value.IsObject())
	{
		auto propertiesObject = propertiesMember->value.GetObjectA();

		auto bbSizeMember = propertiesObject.FindMember("BoundingBoxSize");
		bool aaaaaa = propertiesObject.HasMember("BoundingBoxSize");

		auto bbSizeObject = bbSizeMember->value.GetObjectA();
		if (bbSizeMember->value.IsObject())
		{
			auto bbSizeObject = bbSizeMember->value.GetObjectA();

			auto x = bbSizeObject.FindMember("X");
			auto y = bbSizeObject.FindMember("Y");
			m_Size.X = (float)x->value.GetDouble();
			m_Size.Y = (float)y->value.GetDouble();
		}

		//Call update to recalcuate the TL and BR.
		Update(0.0f);
	}
}

void BoundingBox::RenderProperties()
{
	Collider::RenderProperties();

	ImGui::Text("Box Size: ");
	ImGui::SameLine();
	ImGui::InputFloat("##boxSizeX", &m_Size.X); 
	ImGui::SameLine();
	ImGui::InputFloat("##boxSizeY", &m_Size.Y);
}
