#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

class HTECH_FUNCTION_EXPORT PhysicsProperties
{
public:
	//IsStatic Overrides gravity enabled
	bool  IsStatic = false;
	bool  GravityEnabled = true;
	bool  DragEnabled = true;
	float Mass = 100.0f; //Weight of Entity (kg)
	float DragCoefficient = 0.47f; //Drag coefficient (default to sphere's value of 0.47)
	float SpeedCap = 1000.0f; //Speed cap (u/s)
	float Restitution = 0.5f;
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

	inline PhysicsProperties(
		float mass = 100.0f,
		float restitution = 0.5f,
		float speedCap = 1000.0f,
		float dragCoefficient = 0.47f,
		float staticFriction = 0.4f,
		float dynamicFriction = 0.2f,
		bool dragEnabled = true,
		bool gravityEnabled = true,
		bool isStatic = false)
		: Mass(mass), Restitution(restitution), SpeedCap(speedCap),
		DragCoefficient(dragCoefficient), StaticFriction(staticFriction), DynamicFriction(dynamicFriction), DragEnabled(dragEnabled),
		GravityEnabled(gravityEnabled), IsStatic(isStatic)
	{
			
	};

	inline ~PhysicsProperties()
	{

	};
};