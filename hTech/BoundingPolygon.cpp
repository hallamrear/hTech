#include "pch.h"
#include "BoundingPolygon.h"
#include "Transform.h"
#include "Camera.h"
#include "Console.h"
#include "rapidjson/rapidjson.h"
#include "imgui.h"


BoundingPolygon::BoundingPolygon(Transform& origin) : Collider(origin), mRotation(origin.Rotation)
{
	mType = COLLIDER_TYPE::COLLIDER_POLYGON;
	mOriginalPoints = std::vector<Vector2>();
	PointCount = mOriginalPoints.size();
}

BoundingPolygon::BoundingPolygon(Transform& origin, float& rotation, const std::vector<Vector2> points, const int& pointCount) : Collider(origin), mRotation(rotation)
{
	mType = COLLIDER_TYPE::COLLIDER_POLYGON;
	PointCount = pointCount;
	mOriginalPoints = std::vector<Vector2>();
	mOriginalPoints = points;

	for (size_t i = 0; i < PointCount; i++)
	{
		mTransformedPoints.push_back(mTransform.Position + mOriginalPoints[i]);
	}
}

void BoundingPolygon::Update(float DeltaTime)
{
	PointCount = mTransformedPoints.size();

	for (size_t i = 0; i < mTransformedPoints.size(); i++)
	{
		mTransformedPoints[i] = MathsHelp::RotatePointAroundOriginDegrees(mTransform.Position + mOriginalPoints[i], mRotation, mTransform.Position);
	}
}

void BoundingPolygon::Render(SDL_Renderer& renderer)
{
	if (Console::Query("DrawColliders") != 0)
	{
		SDL_SetRenderDrawColor(&renderer, 0, 255, 255, 255);

		switch (PointCount)
		{
		case 0:
			break;
		case 1:
			SDL_RenderDrawPoint(&renderer, (int)mTransformedPoints[0].X, (int)mTransformedPoints[0].Y);
			break;
		default:
			Vector2 pOne = mTransformedPoints[0];
			Vector2 pTwo;

			for (size_t i = 0; i < PointCount; i++)
			{
				pOne = mTransformedPoints[i % PointCount];
				pTwo = mTransformedPoints[(i + 1) % PointCount];

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
	return Vector2();
}

void BoundingPolygon::GetColliderAsPoints(Vector2 points[]) const
{
	for (size_t i = 0; i < PointCount; i++)
	{
		points[i] = mTransformedPoints[i];
	}
}

void BoundingPolygon::AddPoint(Vector2 point)
{
	mOriginalPoints.push_back(point);
	mTransformedPoints.push_back(mTransform.Position + point);
}

void BoundingPolygon::Serialize(Serializer& writer) const
{
	Collider::Serialize(writer);

	writer.String("PointCount"); writer.Int(PointCount);

	writer.String("Points");
	writer.StartArray();
	for (size_t i = 0; i < PointCount; i++)
	{
		writer.StartObject();
		writer.String("X"); writer.Double(mOriginalPoints[i].X);
		writer.String("Y"); writer.Double(mOriginalPoints[i].Y);
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
	for (size_t i = 0; i < PointCount; i++)
	{
		imguiLabel = mOriginalPoints[i].ToString() + "##point" + std::to_string(i);
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
		mOriginalPoints.erase(mOriginalPoints.begin() + selectedPointIndex);
		mTransformedPoints.erase(mTransformedPoints.begin() + selectedPointIndex);
		PointCount--;
	}
	ImGui::Separator();
}

//0.0f, 100.0f
//50.0f, 0.0f
//25.0f, -100.0f
//-25.0f, -100.0f
//-50.0f, 0.0f