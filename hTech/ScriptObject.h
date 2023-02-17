#pragma once
#include "Entity.h"
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#define EndScript(C) _EndScript(C)
#define StartScript(C) _StartScript(C)
#define _EndScript(C) }; extern "C" {  inline HTECH_FUNCTION_EXPORT ScriptObject* CREATE_FUNCTION(C, Entity* entity) { return new C(entity); } }
#define _StartScript(C) class HTECH_FUNCTION_EXPORT C : public ScriptObject { public: C(Entity* entity) : ScriptObject(entity) {}; private:

#define publicFunction _publicFunction
#define _publicFunction public: inline void
#define privateFunction _privateFunction
#define _privateFunction private: inline void

#define Create_
#define MAKE_FN_NAME(CLASS) Create_##CLASS(Entity* entity)
#define CREATE_FUNCTION(CLASS) MAKE_FN_NAME(CLASS)

class CollisionManifold;
class RigidbodyComponent;

class HTECH_FUNCTION_EXPORT ScriptObject
{
private:
	Entity* const m_ParentEntity;

protected:
	ScriptObject(Entity* const parent);
	Entity* const GetEntity();

public:
	~ScriptObject();

	/// <summary>
	/// Runs once before the first update tick after the start button is pressed.
	/// </summary>
	virtual void Start() = 0;

	/// <summary>
	/// Called whenever the stop button is pressed to reset the entity.
	/// This function should be used to set initial positions and parameters that are not set in the script.
	/// </summary>
	virtual void Reset() = 0;

	/// <summary>
	/// Runs once when the object is enabled.
	/// </summary>
	virtual void OnEnable();

	/// <summary>
	/// Runs once when the object is disabled.
	/// </summary>
	virtual void OnDisable();

	/// <summary>
	/// Runs every frame a collision is occuring.
	/// </summary>
	/// <param name="manifold">Details of the collision this frame.</param>
	/// <param name="other">The other rigidbody involved in this collision</param>
	virtual void OnCollision(const CollisionManifold& manifold, RigidbodyComponent& other);

	/// <summary>
	/// Runs every frame an overlap is occuring.
	/// </summary>
	/// <param name="manifold">Details of the collision this frame.</param>
	/// <param name="other">The other rigidbody involved in this collision</param>
	virtual void OnOverlap(const CollisionManifold& manifold, RigidbodyComponent& other);

	/// <summary>
	///	Runs once every frame.
	/// </summary>
	virtual void Update(float deltaTime) = 0;

	/// <summary>
	///	DO NOT call this function. It is used by the engine to cleanup this script.
	/// </summary>
	virtual void Destroy()
	{
		delete this;
	}
};