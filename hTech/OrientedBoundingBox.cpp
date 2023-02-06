#include "pch.h"
#include "OrientedBoundingBox.h"
#include "Camera.h"
#include "Transform.h"
#include "rapidjson/rapidjson.h"
#include "imgui.h"
#include "Console.h"

OrientedBoundingBox::OrientedBoundingBox(const Transform& transform, float size_x, float size_y)
	: BoundingBox(transform, size_x, size_y)
{
	m_Type = COLLIDER_TYPE::COLLIDER_OBB;
	CalculateRotations();
}

OrientedBoundingBox::~OrientedBoundingBox()
{

}

void OrientedBoundingBox::CalculateRotations()
{
	//if (m_EntityTransform.Rotation != 0.0f && m_EntityTransform.Rotation != 360.0f)
	{
		//float rotation = 360.0f - m_EntityTransform.Rotation;
		float rotation = m_EntityTransform.Rotation;
		m_TopLeft =     Utils::Maths::RotatePointAroundOriginDegrees(Vector2(m_EntityTransform.Position.X - (m_Size.X / 2), m_EntityTransform.Position.Y + (m_Size.Y / 2)), rotation, m_EntityTransform.Position);
		m_BottomLeft =  Utils::Maths::RotatePointAroundOriginDegrees(Vector2(m_EntityTransform.Position.X - (m_Size.X / 2), m_EntityTransform.Position.Y - (m_Size.Y / 2)), rotation, m_EntityTransform.Position);
		m_TopRight =    Utils::Maths::RotatePointAroundOriginDegrees(Vector2(m_EntityTransform.Position.X + (m_Size.X / 2), m_EntityTransform.Position.Y + (m_Size.Y / 2)), rotation, m_EntityTransform.Position);
		m_BottomRight = Utils::Maths::RotatePointAroundOriginDegrees(Vector2(m_EntityTransform.Position.X + (m_Size.X / 2), m_EntityTransform.Position.Y - (m_Size.Y / 2)), rotation, m_EntityTransform.Position);
	}
}

void OrientedBoundingBox::Update(float DeltaTime)
{
	CalculateRotations();
}

void OrientedBoundingBox::Render(SDL_Renderer& renderer)
{
	if (Console::Query("DrawColliders") != 0)
	{
		SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);

		const Vector2 TopLeft_ScreenSpace = Camera::WorldToScreen(m_TopLeft);
		const Vector2 TopRight_ScreenSpace = Camera::WorldToScreen(m_TopRight);
		const Vector2 BottomLeft_ScreenSpace = Camera::WorldToScreen(m_BottomLeft);
		const Vector2 BottomRight_ScreenSpace = Camera::WorldToScreen(m_BottomRight);

		//Top
		SDL_RenderDrawLine(&renderer, round(TopLeft_ScreenSpace.X), round(TopLeft_ScreenSpace.Y), round(TopRight_ScreenSpace.X), round(TopRight_ScreenSpace.Y));
		//Bot
		SDL_RenderDrawLine(&renderer, round(BottomLeft_ScreenSpace.X), round(BottomLeft_ScreenSpace.Y), round(BottomRight_ScreenSpace.X), round(BottomRight_ScreenSpace.Y));
		//Left
		SDL_RenderDrawLine(&renderer, round(TopLeft_ScreenSpace.X), round(TopLeft_ScreenSpace.Y), round(BottomLeft_ScreenSpace.X), round(BottomLeft_ScreenSpace.Y));
		//Right
		SDL_RenderDrawLine(&renderer, round(TopRight_ScreenSpace.X), round(TopRight_ScreenSpace.Y), round(BottomRight_ScreenSpace.X), round(BottomRight_ScreenSpace.Y));
	}
}

//Fills array with points of the box.
void OrientedBoundingBox::GetColliderAsPoints(Vector2 points[]) const
{
	points[0] = m_BottomLeft;
	points[1] = m_BottomRight;
	points[2] = m_TopRight;
	points[3] = m_TopLeft;
}

void OrientedBoundingBox::Serialize(Serializer& writer) const
{
	BoundingBox::Serialize(writer);
}

void OrientedBoundingBox::Deserialize(SerializedValue& value)
{
	BoundingBox::Deserialize(value);
}

void OrientedBoundingBox::RenderProperties()
{
	BoundingBox::RenderProperties();
}
