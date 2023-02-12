#include "pch.h"
#include "BoundingPolygon.h"
#include "Transform.h"
#include "Camera.h"
#include "Console.h"
#include "imgui.h"

BoundingPolygon::BoundingPolygon(const Transform& origin) : Collider(origin), m_Rotation(origin.Rotation)
{
	m_Type = COLLIDER_TYPE::COLLIDER_POLYGON;
	m_OriginalPoints = std::vector<Vector2>();
	m_PointCount = m_OriginalPoints.size();
}

BoundingPolygon::BoundingPolygon(const Transform& origin, const std::vector<Vector2> points, const int& pointCount) : Collider(origin), m_Rotation(origin.Rotation)
{
	m_Type = COLLIDER_TYPE::COLLIDER_POLYGON;
	m_PointCount = pointCount;
	m_OriginalPoints = std::vector<Vector2>();
	m_OriginalPoints = points;

	for (size_t i = 0; i < m_PointCount; i++)
	{
		m_TransformedPoints.push_back(m_EntityTransform.Position + m_OriginalPoints[i]);
	}
}

void BoundingPolygon::Update(float DeltaTime)
{
	m_PointCount = m_TransformedPoints.size();

	for (size_t i = 0; i < m_TransformedPoints.size(); i++)
	{
		m_TransformedPoints[i] = Utils::Maths::RotatePointAroundOriginDegrees(m_EntityTransform.Position + m_OriginalPoints[i], m_Rotation, m_EntityTransform.Position);
	}
}

void BoundingPolygon::Render(SDL_Renderer& renderer)
{
	if (Console::Query("DrawColliders") != 0)
	{
		SDL_SetRenderDrawColor(&renderer, 0, 0, 255, 255);

		switch (m_PointCount)
		{
		case 0:
			break;
		case 1:
			SDL_RenderDrawPoint(&renderer, (int)m_TransformedPoints[0].X, (int)m_TransformedPoints[0].Y);
			break;
		default:
			Vector2 pOne = m_TransformedPoints[0];
			Vector2 pTwo;

			for (size_t i = 0; i < m_PointCount; i++)
			{
				pOne = m_TransformedPoints[i % m_PointCount];
				pTwo = m_TransformedPoints[(i + 1) % m_PointCount];

				Vector2 screenSpaceOne = Camera::WorldToScreen(pOne);
				Vector2 screenSpaceTwo = Camera::WorldToScreen(pTwo);

				SDL_RenderDrawLine(&renderer, (int)screenSpaceOne.X, (int)screenSpaceOne.Y, (int)screenSpaceTwo.X, (int)screenSpaceTwo.Y);
			}
			break;
		}
	}
}

Vector2 BoundingPolygon::FindFurthestPoint(Vector2 direction) const
{
	if (m_PointCount <= 0)
	{
		return Vector2();
	}

	Vector2 directionNorm = direction.GetNormalized();

	float furthestDistance = -100000.0f;
	int furthestVertexIndex = -1;
	float projectedDistance = -100000.0f;

	for (size_t i = 0; i < m_PointCount; i++)
	{
		Vector2 vertex = m_TransformedPoints[i];

		projectedDistance = Utils::Maths::Dot(vertex, directionNorm);

		if (projectedDistance >= furthestDistance)
		{
			furthestDistance = projectedDistance;
			furthestVertexIndex = i;
		}
	}

	if (furthestVertexIndex == -1)
	{
		throw std::out_of_range("Furthest Index is -1. Calculation error somewhere here.");
	}

	return m_TransformedPoints[furthestVertexIndex];
}

void BoundingPolygon::GetColliderAsPoints(Vector2 points[]) const
{
	for (size_t i = 0; i < m_PointCount; i++)
	{
		points[i] = m_TransformedPoints[i];
	}
}

const size_t BoundingPolygon::GetPointCount() const
{
	return m_PointCount;
}

void BoundingPolygon::AddPoint(Vector2 point)
{
	m_OriginalPoints.push_back(point);
	m_TransformedPoints.push_back(m_EntityTransform.Position + point);
}

void BoundingPolygon::Serialize(Serializer& writer) const
{
	Collider::Serialize(writer);

	writer.String("PointCount"); writer.Int(m_PointCount);

	writer.String("Points");
	writer.StartArray();
	for (size_t i = 0; i < m_PointCount; i++)
	{
		writer.StartObject();
		writer.String("X"); writer.Double(m_OriginalPoints[i].X);
		writer.String("Y"); writer.Double(m_OriginalPoints[i].Y);
		writer.EndObject();
	}
	writer.EndArray();
}

void BoundingPolygon::Deserialize(SerializedValue& value)
{
	Collider::Deserialize(value);
	auto propertiesMember = value.FindMember("Physics Properties");

	if (propertiesMember->value.IsObject())
	{
		auto properties = propertiesMember->value.GetObjectA();

		auto pointCountMember = properties.FindMember("PointCount");
		if (pointCountMember->value.IsInt())
		{
			int pointCount = pointCountMember->value.GetInt();

			auto colliderPointsMember = properties.FindMember("Points");

			if (colliderPointsMember->value.IsArray())
			{
				auto points = colliderPointsMember->value.GetArray();

				for (size_t i = 0; i < points.Size(); i++)
				{
					float x = (float)points[i]["X"].GetDouble();
					float y = (float)points[i]["Y"].GetDouble();
					AddPoint(Vector2(x, y));
				}
			}
		}
		
	}
}

void BoundingPolygon::RenderProperties()
{
	static Vector2 newPoint;

	//Display existing points

	std::string imguiLabel = "";
	static int selectedPointIndex = -1;
	ImGui::BeginListBox("Points");
	for (size_t i = 0; i < m_PointCount; i++)
	{
		imguiLabel = m_OriginalPoints[i].ToString() + "##point" + std::to_string(i);
		bool selected = (i == selectedPointIndex);
		if (ImGui::Selectable(imguiLabel.c_str(), &selected))
		{
			selectedPointIndex = i;
			
			if (selected)
				ImGui::SetItemDefaultFocus();
		}
	}
	ImGui::EndListBox();

	//Create new point
	ImGui::Separator();
	ImGui::Text("New Point X:"); 
	ImGui::SameLine(); 
	ImGui::InputFloat("##newPointX", &newPoint.X);
	ImGui::SameLine();
	ImGui::Text("Y:"); 
	ImGui::SameLine();
	ImGui::InputFloat("##newPointY", &newPoint.Y);

	if (ImGui::Button("Add Point"))
	{
		AddPoint(newPoint);
		newPoint.X = 0.0f;
		newPoint.Y = 0.0f;
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove selected point"))
	{
		if (selectedPointIndex != -1)
		{
			m_OriginalPoints.erase(m_OriginalPoints.begin() + selectedPointIndex);
			m_TransformedPoints.erase(m_TransformedPoints.begin() + selectedPointIndex);
			m_PointCount--;
		}
	}
	ImGui::Separator();
}

//0.0f, 100.0f
//50.0f, 0.0f
//25.0f, -100.0f
//-25.0f, -100.0f
//-50.0f, 0.0f