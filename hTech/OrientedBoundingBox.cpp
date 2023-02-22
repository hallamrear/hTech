#include "pch.h"
#include "OrientedBoundingBox.h"
#include "Camera.h"
#include "Transform.h"
#include "rapidjson/rapidjson.h"
#include "imgui.h"
#include "Console.h"
#include "IRenderer.h"

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

void OrientedBoundingBox::Render(IRenderer& renderer)
{
	if (Console::Query("DrawColliders") != 0)
	{
		renderer.SetPrimativeDrawColour(Colour::Red);
		renderer.Render_Line(m_TopLeft, m_TopRight);
		renderer.Render_Line(m_BottomLeft, m_BottomRight);
		renderer.Render_Line(m_TopLeft, m_BottomLeft);
		renderer.Render_Line(m_TopRight, m_BottomRight);
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
