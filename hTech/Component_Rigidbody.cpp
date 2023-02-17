#include "pch.h"
#include "Component_Rigidbody.h"
#include "Component_Script.h"
#include "Entity.h"
#include "PhysicsWorld.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "BoundingPolygon.h"
#include "OrientedBoundingBox.h"
#include "Log.h"

RigidbodyComponent::RigidbodyComponent(Entity& entity) : Component("Rigidbody Component", entity)
{
	m_IsStatic = true;
	m_GravityEnabled = true;
	m_DragEnabled = true;
	m_Mass = 100.0f; 
	m_DragCoefficient = 0.47f;
	m_SpeedCap = 1000.0f;
	m_Restitution = 0.5f;
	m_StaticFriction = 0.4f;
	m_DynamicFriction = 0.2f;
	m_Collider = nullptr;

	CalculateInverseMass();

	Physics::RegisterRigidbody(this);
}

RigidbodyComponent::~RigidbodyComponent()
{
	Physics::DeregisterRigidbody(this);

	m_GravityEnabled  = false;
	m_DragEnabled	 = false;
	m_Mass			 = 0.0f;
	m_InverseMass	 = 0.0f;
	m_DragCoefficient = 0.0f;
	m_SpeedCap		 = 0.0f;
	m_Restitution	 = 0.0f;
	m_Velocity		 = Vector2();
	m_Acceleration	 = Vector2();
	m_NetForce		 = Vector2();
	m_ExternalForce	 = Vector2();
	m_IsStatic = true;
}

/// <summary>
/// Do not call this function. It will mess up the physics timestep calculations.
/// </summary>
/// <param name="DeltaTime"></param>
void RigidbodyComponent::PhysicsUpdate(float DeltaTime)
{
	CalculateInverseMass();

	if (isnan(abs(m_ParentEntity.GetTransform().Position.X)))
	{
		assert(0);
	}

	if (m_IsStatic)
		return;

	if (GetDragEnabled())
	{
		///Drag
		Vector2 dragForce;
		dragForce.X = -m_DragCoefficient * m_Velocity.X;
		dragForce.Y = -m_DragCoefficient * m_Velocity.Y;
		m_NetForce += dragForce;

		Vector2 frictionForce;
		float mew = 2.5f;
		if (m_Velocity.GetMagnitude() < mew * DeltaTime) // Make sure the friction doesn't overextend.
			frictionForce = ((m_Velocity * -1) / DeltaTime);
		else
			frictionForce = (m_Velocity.GetNormalized() * -1) * mew; // getNormalized() returns the unit vector.

		frictionForce = ((m_Velocity * -1) * mew);

		m_NetForce += frictionForce;
	}

	///External
	//No need for gravity
	if (GetGravityEnabled())
	{
		//HARDCODED Gravity value
		m_NetForce += (Vector2(0.0f, -9.81f) * m_Mass);
	}

	m_NetForce += m_ExternalForce;

	///Acceleration
	m_Acceleration = m_NetForce * m_InverseMass;

	///Update Position
	Vector2 velSum = m_Acceleration * (DeltaTime);
	m_Velocity += velSum;
	m_ParentEntity.GetTransform().Position += m_Velocity;

	///Speed Cap
	//Capping - X
	m_Velocity.X > m_SpeedCap ? m_Velocity.X = m_SpeedCap : m_Velocity.X;
	m_Velocity.X < -m_SpeedCap ? m_Velocity.X = -m_SpeedCap : m_Velocity.X;

	//Capping - Y
	m_Velocity.Y > m_SpeedCap  ? m_Velocity.Y = m_SpeedCap  : m_Velocity.Y;
	m_Velocity.Y < -m_SpeedCap ? m_Velocity.Y = -m_SpeedCap : m_Velocity.Y;

	float mag = m_Velocity.GetMagnitude();
	if (mag < 0.001f)
		m_Velocity = Vector2(0.0f, 0.0f);

	m_NetForce = Vector2();
	m_ExternalForce = Vector2();
}

void RigidbodyComponent::DestroyCollider()
{
	if (m_Collider)
	{
		delete m_Collider;
		m_Collider = nullptr;
	}
}

void RigidbodyComponent::SetCollider(COLLIDER_TYPE type)
{
	if (m_Collider != nullptr)
	{
		if (type != m_Collider->GetType())
		{
			DestroyCollider();
		}
	}

	switch (type)
	{
	default:
	case COLLIDER_TYPE::COLLIDER_UNKNOWN:
		m_Collider = nullptr;
		break;
	case COLLIDER_TYPE::COLLIDER_AABB:
		m_Collider = new BoundingBox(m_ParentEntity.GetTransform(), 64, 64);
		break;
	case COLLIDER_TYPE::COLLIDER_SPHERE:
		m_Collider = new BoundingSphere(m_ParentEntity.GetTransform(), 64);
		break;
	case COLLIDER_TYPE::COLLIDER_OBB:
		m_Collider = new OrientedBoundingBox(m_ParentEntity.GetTransform(), 64, 64);
		break;
	case COLLIDER_TYPE::COLLIDER_POLYGON:
		m_Collider = new BoundingPolygon(m_ParentEntity.GetTransform());
		break;
	}
}

Collider* const RigidbodyComponent::GetCollider()
{
	if (m_Collider != nullptr)
	{
		return m_Collider;
	}
	else
	{
		return nullptr;
	}
}

void RigidbodyComponent::AddForce(Vector2 force)
{
	m_ExternalForce += force;
}

void RigidbodyComponent::AddForce(float X, float Y)
{
	m_ExternalForce.X += X;
	m_ExternalForce.Y += Y;
}

void RigidbodyComponent::RenderProperties()
{
	bool isStatic = GetIsStatic();
	ImGui::Checkbox("Static", &isStatic);
	SetIsStatic(isStatic);
	ImGui::Checkbox("Gravity Enabled", &m_GravityEnabled);
	ImGui::Checkbox("Drag Enabled", &m_DragEnabled);
	
	static const std::string colldierStrs[5] = { "None", "AABB", "BROKEN - Sphere", "OBB", "Polygon" };
	static std::string currentItem;

	if (m_Collider)
	{
		currentItem = colldierStrs[(int)m_Collider->GetType() + 1];
	}
	else
	{
		currentItem = colldierStrs[0];
	}

	ImGui::Separator();
	if (ImGui::BeginCombo("Collider Type", currentItem.c_str()))
	{
		for (int n = 0; n < IM_ARRAYSIZE(colldierStrs); n++)
		{
			bool is_selected = (currentItem == colldierStrs[n].c_str()); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(colldierStrs[n].c_str(), is_selected))
			{
				if (currentItem != colldierStrs[n])
				{
					currentItem = colldierStrs[n];
					SetCollider((COLLIDER_TYPE)(n - 1));
					if (is_selected)
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
				}
			}
		}
		ImGui::EndCombo();
	}

	if (m_Collider != nullptr)
	{
		m_Collider->RenderProperties();
	}
	ImGui::Separator();

	ImGui::InputFloat("Mass (kg)", &m_Mass);
	ImGui::InputFloat("Drag Coefficient", &m_DragCoefficient);
	ImGui::InputFloat("Speed cap (u/s)", &m_SpeedCap);
	ImGui::InputFloat("Restitution", &m_Restitution);

	ImGui::Text("      Velocity -> X: %f, Y: %f", m_Velocity.X, m_Velocity.Y);
	ImGui::Text("  Acceleration -> X: %f, Y: %f", m_Acceleration.X, m_Acceleration.Y);
	ImGui::Text("     Net Force -> X: %f, Y: %f", m_NetForce.X, m_NetForce.Y);
	ImGui::Text("External Force -> X: %f, Y: %f", m_ExternalForce.X, m_ExternalForce.Y);

	if (ImGui::Button("Zero velocity and acceleration"))
	{
		m_Velocity = Vector2(0.0f, 0.0f);
		m_Acceleration = Vector2(0.0f, 0.0f);
	}
}

void RigidbodyComponent::CalculateInverseMass()
{
	if (GetIsStatic())
		m_InverseMass = 0.0f;
	else
		m_InverseMass = 1.0f / m_Mass;
}

void RigidbodyComponent::OnCollision(const CollisionManifold& manifold, RigidbodyComponent& other)
{
	ScriptComponent* script = m_ParentEntity.GetComponent<ScriptComponent>();
	if (script != nullptr)
	{
		script->OnCollision(manifold, other);
	}
}

void RigidbodyComponent::OnOverlap(const CollisionManifold& manifold, RigidbodyComponent& other)
{
	ScriptComponent* script = m_ParentEntity.GetComponent<ScriptComponent>();
	if (script != nullptr)
	{
		script->OnOverlap(manifold, other);
	}
}

void RigidbodyComponent::Update(float DeltaTime)
{
	if (m_Collider)
	{
		m_Collider->Update(DeltaTime);
	}
}

void RigidbodyComponent::Render(SDL_Renderer& renderer)
{
	if (m_Collider)
	{
		m_Collider->Render(renderer);
	}
}

void RigidbodyComponent::Serialize(Serializer& writer) const
{
	Component::Serialize(writer);

	writer.String("Physics Properties");
	writer.StartObject();

	writer.String("Collider Type");
	if (m_Collider != nullptr)
	{
		int value = (int)m_Collider->GetType();
		writer.Int(value);
		m_Collider->Serialize(writer);
	}
	else
	{
		writer.Null();
	}

	writer.String("Static");			writer.Bool(m_IsStatic);
	writer.String("Gravity Enabled");   writer.Bool(m_GravityEnabled);
	writer.String("Drag Enabled");      writer.Bool(m_DragEnabled);
	writer.String("Mass");				writer.Double(m_Mass);
	writer.String("Inverse Mass");		writer.Double(m_InverseMass);
	writer.String("Drag Coefficient");	writer.Double(m_DragCoefficient);
	writer.String("Speed cap");			writer.Double(m_SpeedCap);
	writer.String("Restitution");		writer.Double(m_Restitution);
	writer.String("Static Friction");		writer.Double(m_StaticFriction);
	writer.String("Dynamic Friction");		writer.Double(m_DynamicFriction);

	//writer.String("Velocity"); 
	//writer.StartObject();
	//writer.String("X");					writer.Double((double)m_Velocity.X);
	//writer.String("Y");					writer.Double((double)m_Velocity.Y);
	//writer.EndObject();

	//writer.String("Acceleration"); 
	//writer.StartObject();
	//writer.String("X");					writer.Double((double)m_Acceleration.X);
	//writer.String("Y");					writer.Double((double)m_Acceleration.Y);
	//writer.EndObject();

	//writer.String("Net Force"); 
	//writer.StartObject();
	//writer.String("X");					writer.Double((double)m_NetForce.X);
	//writer.String("Y");					writer.Double((double)m_NetForce.Y);
	//writer.EndObject();

	//writer.String("External Force"); 
	//writer.StartObject();
	//writer.String("X");					writer.Double((double)m_ExternalForce.X);
	//writer.String("Y");					writer.Double((double)m_ExternalForce.Y);
	//writer.EndObject();

	writer.EndObject();

}

void RigidbodyComponent::Deserialize(SerializedValue& value)
{
	Component::Deserialize(value);

	auto propertiesMember = value.FindMember("Physics Properties");

	if (propertiesMember->value.IsObject())
	{
		auto properties = propertiesMember->value.GetObjectA();

		auto colliderMember = properties.FindMember("Collider Type");
		if (colliderMember->value.IsInt())
		{
			int colliderValue = colliderMember->value.GetInt();
			SetCollider((COLLIDER_TYPE)colliderValue);
			if (m_Collider != nullptr)
			{
				m_Collider->Deserialize(value);
			}
		}
		else
		{
			SetCollider(COLLIDER_TYPE::COLLIDER_UNKNOWN);
		}

		auto staticMember = properties.FindMember("Static");
		if (staticMember->value.IsBool())
			m_IsStatic = staticMember->value.GetBool();

		auto gravityMember = properties.FindMember("Gravity Enabled");
		if (gravityMember->value.IsBool())
			m_GravityEnabled = gravityMember->value.GetBool();

		auto dragMember = properties.FindMember("Drag Enabled");       
		if (dragMember->value.IsBool()) 
			m_DragEnabled = dragMember->value.GetBool();

		auto massMember = properties.FindMember("Mass");
		if  (massMember->value.IsDouble()) 
			m_Mass = (float)massMember->value.GetDouble();

		auto invmassMember = properties.FindMember("Inverse Mass");
		if (invmassMember->value.IsDouble())
			m_InverseMass = (float)invmassMember->value.GetDouble();

		auto dragcoeffMember = properties.FindMember("Drag Coefficient");
		if (dragcoeffMember->value.IsDouble())
			m_DragCoefficient = (float)dragcoeffMember->value.GetDouble();

		auto speedCapMember = properties.FindMember("Speed cap");
		if (speedCapMember->value.IsDouble())
			m_SpeedCap = (float)speedCapMember->value.GetDouble();

		auto restMember = properties.FindMember("Restitution");
		if (restMember->value.IsDouble())
			m_Restitution = (float)restMember->value.GetDouble();


		auto sFrictionMember = properties.FindMember("Static Friction");
		if (sFrictionMember->value.IsDouble())
			m_StaticFriction = (float)sFrictionMember->value.GetDouble();


		auto dFrictionMember = properties.FindMember("Dynamic Friction");
		if (dFrictionMember->value.IsDouble())
			m_DynamicFriction = (float)dFrictionMember->value.GetDouble();

		//auto externalForceMember = properties.FindMember("External Force");
		//if (externalForceMember->value.IsObject())
		//{
		//	m_ExternalForce.X = (float)externalForceMember->value["X"].GetDouble();
		//	m_ExternalForce.Y = (float)externalForceMember->value["Y"].GetDouble();
		//}

		//auto netForceMember = properties.FindMember("Net Force");
		//if (netForceMember->value.IsObject())
		//{
		//	m_NetForce.X = (float)netForceMember->value["X"].GetDouble();
		//	m_NetForce.Y = (float)netForceMember->value["Y"].GetDouble();
		//}

		//auto accelerationMember = properties.FindMember("Acceleration");
		//if (accelerationMember->value.IsObject())
		//{
		//	m_Acceleration.X = (float)accelerationMember->value["X"].GetDouble();
		//	m_Acceleration.Y = (float)accelerationMember->value["Y"].GetDouble();
		//}

		//auto velocityMember = properties.FindMember("Velocity");
		//if (velocityMember->value.IsObject())
		//{
		//	m_Velocity.X = (float)velocityMember->value["X"].GetDouble();
		//	m_Velocity.Y = (float)velocityMember->value["Y"].GetDouble();
		//}
	}
}
