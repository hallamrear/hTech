#include "pch.h"
#include "Component_Rigidbody.h"
#include "Component_Script.h"
#include "Entity.h"
#include "PhysicsWorld.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "BoundingPolygon.h"
#include "OrientedBoundingBox.h"
#include "Game.h"

RigidbodyComponent::RigidbodyComponent(Entity& entity) : Component("Rigidbody Component", entity)
{
	mIsStatic = false;
	mGravityEnabled = true;
	mDragEnabled = true;
	mMass = 100.0f; 
	mDragCoefficient = 0.47f;
	mSpeedCap = 1000.0f;
	mRestitution = 0.5f;
	mCollider = nullptr;

	CalculateInverseMass();

	Physics::RegisterRigidbody(this);
}

RigidbodyComponent::~RigidbodyComponent()
{
	Physics::DeregisterRigidbody(this);

	mGravityEnabled  = false;
	mDragEnabled	 = false;
	mMass			 = 0.0f;
	mInverseMass	 = 0.0f;
	mDragCoefficient = 0.0f;
	mSpeedCap		 = 0.0f;
	mRestitution	 = 0.0f;
	mVelocity		 = Vector2();
	mAcceleration	 = Vector2();
	mNetForce		 = Vector2();
	mExternalForce	 = Vector2();
	mIsStatic = false;
}

/// <summary>
/// Do not call this function. It will mess up the physics timestep calculations.
/// </summary>
/// <param name="DeltaTime"></param>
void RigidbodyComponent::PhysicsUpdate(float DeltaTime)
{
	CalculateInverseMass();

	if (mIsStatic)
		return;

	if (GetDragEnabled())
	{
		///Drag
		Vector2 dragForce;
		dragForce.X = -mDragCoefficient * mVelocity.X;
		dragForce.Y = -mDragCoefficient * mVelocity.Y;
		mNetForce += dragForce;

		Vector2 frictionForce;
		float mew = 2.5f;
		if (mVelocity.GetMagnitude() < mew * DeltaTime) // Make sure the friction doesn't overextend.
			frictionForce = ((mVelocity * -1) / DeltaTime);
		else
			frictionForce = (mVelocity.GetNormalized() * -1) * mew; // getNormalized() returns the unit vector.

		frictionForce = ((mVelocity * -1) * mew);

		mNetForce += frictionForce;
	}

	///External
	//No need for gravity
	if (GetGravityEnabled())
	{
		//HARDCODED Gravity value
		mNetForce += (Vector2(0.0f, -9.81f) * mMass);
	}

	mNetForce += mExternalForce;

	///Acceleration
	mAcceleration = mNetForce / mMass;

	///Update Position
	mVelocity += mAcceleration * static_cast<float>(DeltaTime);
	Parent.GetTransform().Position += mVelocity;

	///Speed Cap
	//Capping - X
	if (mVelocity.X > mSpeedCap)
		mVelocity.X = mSpeedCap;
	else if (mVelocity.X < -mSpeedCap)
		mVelocity.X = -mSpeedCap;
	//Capping - Y
	if (mVelocity.Y > mSpeedCap)
		mVelocity.Y = mSpeedCap;
	else if (mVelocity.Y < -mSpeedCap)
		mVelocity.Y = -mSpeedCap;

	mNetForce = Vector2();
	mExternalForce = Vector2();
}

void RigidbodyComponent::DestroyCollider()
{
	if (mCollider)
	{
		delete mCollider;
		mCollider = nullptr;
	}
}

void RigidbodyComponent::SetCollider(COLLIDER_TYPE type)
{
	if (mCollider != nullptr)
	{
		if (type != mCollider->mType)
		{
			DestroyCollider();
		}
	}

	switch (type)
	{
	default:
	case COLLIDER_TYPE::COLLIDER_UNKNOWN:
		mCollider = nullptr;
		break;
	case COLLIDER_TYPE::COLLIDER_AABB:
		mCollider = new BoundingBox(Parent.GetTransform(), 64, 64);
		break;
	case COLLIDER_TYPE::COLLIDER_SPHERE:
		mCollider = new BoundingSphere(Parent.GetTransform(), 64);
		break;
	case COLLIDER_TYPE::COLLIDER_OBB:
		mCollider = new OrientedBoundingBox(Parent.GetTransform(), 64, 64);
		break;
	case COLLIDER_TYPE::COLLIDER_POLYGON:
		//TODO : Implement
		break;
	}
}

Collider* const RigidbodyComponent::GetCollider()
{
	if (mCollider != nullptr)
	{
		return mCollider;
	}
	else
	{
		return nullptr;
	}
}

void RigidbodyComponent::AddForce(Vector2 force)
{
	mExternalForce += force;
}

void RigidbodyComponent::AddForce(float X, float Y)
{
	mExternalForce.X += X;
	mExternalForce.Y += Y;
}

void RigidbodyComponent::RenderProperties()
{
	//IMPLEMENT Component properties panel.
}

void RigidbodyComponent::CalculateInverseMass()
{
	if (GetIsStatic())
		mInverseMass = 0.0f;
	else
		mInverseMass = 1.0f / mMass;
}

void RigidbodyComponent::OnCollision(const CollisionManifold& manifold, RigidbodyComponent& other)
{
	ScriptComponent* script = Parent.GetComponent<ScriptComponent>();
	if (script != nullptr)
	{
		script->OnCollision(manifold, other);
	}
}

void RigidbodyComponent::OnOverlap(const CollisionManifold& manifold, RigidbodyComponent& other)
{
	ScriptComponent* script = Parent.GetComponent<ScriptComponent>();
	if (script != nullptr)
	{
		script->OnOverlap(manifold, other);
	}
}

void RigidbodyComponent::Update(float DeltaTime)
{
	if (mCollider)
	{
		mCollider->Update(DeltaTime);
	}
}

void RigidbodyComponent::Render(SDL_Renderer& renderer)
{
	if (mCollider)
	{
		mCollider->Render(renderer);
	}
}

void RigidbodyComponent::Serialize(Serializer& writer) const
{
	Component::Serialize(writer);

	writer.String("Physics Properties");
	writer.StartObject();

	writer.String("Collider Type");
	if (mCollider != nullptr)
	{
		writer.Int((int)mCollider->mType);
	}
	else
	{
		writer.Null();
	}

	writer.String("Static");			writer.Bool(mIsStatic);
	writer.String("Gravity Enabled");    writer.Bool(mGravityEnabled);
	writer.String("Drag Enabled");       writer.Bool(mDragEnabled);
	writer.String("Mass");					writer.Double(mMass);
	writer.String("Inverse Mass");					writer.Double(mInverseMass);
	writer.String("Drag Coefficient");					writer.Double(mDragCoefficient);
	writer.String("Speed cap");					writer.Double(mSpeedCap);
	writer.String("Restitution");					writer.Double(mRestitution);

	writer.String("Velocity"); 
	writer.StartObject();
	writer.String("X");					writer.Double((double)mVelocity.X);
	writer.String("Y");					writer.Double((double)mVelocity.Y);
	writer.EndObject();

	writer.String("Acceleration"); 
	writer.StartObject();
	writer.String("X");					writer.Double((double)mAcceleration.X);
	writer.String("Y");					writer.Double((double)mAcceleration.Y);
	writer.EndObject();

	writer.String("Net Force"); 
	writer.StartObject();
	writer.String("X");					writer.Double((double)mNetForce.X);
	writer.String("Y");					writer.Double((double)mNetForce.Y);
	writer.EndObject();

	writer.String("External Force"); 
	writer.StartObject();
	writer.String("X");					writer.Double((double)mExternalForce.X);
	writer.String("Y");					writer.Double((double)mExternalForce.Y);
	writer.EndObject();

	writer.EndObject();

}

void RigidbodyComponent::Deserialize(SerializedValue& value)
{
	Component::Deserialize(value);

	//TODO FINISH THIS AND ANIMATION AND I THINK ITLL BE NEARLY DONE WITH SOME TESTING;





	auto propertiesMember = value.FindMember("Properties");

	if (propertiesMember->value.IsObject())
	{
		auto properties = propertiesMember->value.GetObjectA();

		auto colliderMember = properties.FindMember("Collider Type");
		if (colliderMember->value.IsInt())
		{
			SetCollider((COLLIDER_TYPE)value.GetInt());
		}
		else
		{
			SetCollider(COLLIDER_TYPE::COLLIDER_UNKNOWN);
		}

		auto staticMember = properties.FindMember("Static");
		if (staticMember->value.IsBool())
			mIsStatic = staticMember->value.GetBool();

		auto gravityMember = properties.FindMember("Gravity Enabled");
		if (gravityMember->value.IsBool())
			mGravityEnabled = gravityMember->value.GetBool();

		auto dragMember = properties.FindMember("Drag Enabled");       
		if (dragMember->value.IsBool()) 
			mDragEnabled = dragMember->value.GetBool();

		auto massMember = properties.FindMember("Mass");
		if  (massMember->value.IsDouble()) 
			mMass = massMember->value.GetDouble();

		auto invmassMember = properties.FindMember("Inverse Mass");
		if (invmassMember->value.IsDouble())
			mInverseMass = invmassMember->value.GetDouble();

		auto dragcoeffMember = properties.FindMember("Drag Coefficient");
		if (dragcoeffMember->value.IsDouble())
			mDragCoefficient = dragcoeffMember->value.GetDouble();

		auto speedCapMember = properties.FindMember("Speed cap");
		if (speedCapMember->value.IsBool())
			mSpeedCap = speedCapMember->value.GetDouble();

		auto restMember = properties.FindMember("Restitution");
		if (restMember->value.IsBool())
			mRestitution = restMember->value.GetDouble();

		auto externalForceMember = properties.FindMember("External Force");
		if (externalForceMember->value.IsObject())
		{
			mExternalForce.X = (float)externalForceMember->value["X"].GetDouble();
			mExternalForce.Y = (float)externalForceMember->value["Y"].GetDouble();
		}

		auto netForceMember = properties.FindMember("Net Force");
		if (netForceMember->value.IsObject())
		{
			mNetForce.X = (float)netForceMember->value["X"].GetDouble();
			mNetForce.Y = (float)netForceMember->value["Y"].GetDouble();
		}

		auto accelerationMember = properties.FindMember("Acceleration");
		if (accelerationMember->value.IsObject())
		{
			mAcceleration.X = (float)accelerationMember->value["X"].GetDouble();
			mAcceleration.Y = (float)accelerationMember->value["Y"].GetDouble();
		}

		auto velocityMember = properties.FindMember("Velocity");
		if (velocityMember->value.IsObject())
		{
			mVelocity.X = (float)velocityMember->value["X"].GetDouble();
			mVelocity.Y = (float)velocityMember->value["Y"].GetDouble();
		}
	}
}
