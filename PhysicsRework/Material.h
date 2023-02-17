#pragma once

class PhysicsMaterial
{
public:
	/// <summary>
	/// Coefficient of Restitution
	/// </summary>
	float Restitution = 0.02f;
	/// <summary>
	/// Static friction value
	/// </summary>
	float StaticFriction = 0.4f;
	/// <summary>
	/// Dynamic friction value
	/// </summary>
	float DynamicFriction = 0.2f;

	PhysicsMaterial()
	{

	}

	PhysicsMaterial(float restitution, float staticFriction = 0.4f, float dynamicFriction = 0.2f)
	{
		Restitution = restitution;
		StaticFriction = staticFriction;
		DynamicFriction = dynamicFriction;
	}
};

//const PhysicsMaterial Rock = PhysicsMaterial(0.6f, 0.1f);
//const PhysicsMaterial Wood = PhysicsMaterial(0.3f, 0.2f);
//const PhysicsMaterial Metal = PhysicsMaterial(1.2f, 0.05f);
//const PhysicsMaterial BouncyBall = PhysicsMaterial(0.3f, 0.8f);
//const PhysicsMaterial SuperBall = PhysicsMaterial(0.3f, 0.95f);
//const PhysicsMaterial Pillow = PhysicsMaterial(0.1f, 0.2f);
//const PhysicsMaterial Static = PhysicsMaterial(0.0f, 0.4f);