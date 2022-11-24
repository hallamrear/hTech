#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "Component.h"
#include "ScriptLoader.h"

class ScriptObject;
class CollisionManifold;
class RigidbodyComponent;

class HTECH_FUNCTION_EXPORT ScriptComponent :
    public Component
{
private:
    ScriptObject* mScriptObject; 
    
protected:
    virtual void Destroy();

public:
    ScriptComponent(Entity& entity);
    ~ScriptComponent();

	/// <summary>
	/// CALLS THE FUNCTION IN THE EXTERNAL SCRIPT!
	/// Runs at the when the play button is pressed.
	/// </summary>
	void Start();

	/// <summary>
	/// CALLS THE FUNCTION IN THE EXTERNAL SCRIPT!
	/// Runs once when the object is enabled.
	/// </summary>
	void OnEnable();

	/// <summary>
	/// CALLS THE FUNCTION IN THE EXTERNAL SCRIPT!
	/// Runs once when the object is disabled.
	/// </summary>
	void OnDisable();

	/// <summary>
	/// CALLS THE FUNCTION IN THE EXTERNAL SCRIPT!
	/// Runs every frame a collision is occuring.
	/// </summary>
	/// <param name="manifold">Details of the collision this frame.</param>
	/// <param name="other">The other rigidbody involved in this collision</param>
	void OnCollision(const CollisionManifold& manifold, RigidbodyComponent& other);

	/// <summary>
	/// CALLS THE FUNCTION IN THE EXTERNAL SCRIPT!
	/// Runs every frame an overlap is occuring.
	/// </summary>
	/// <param name="manifold">Details of the collision this frame.</param>
	/// <param name="other">The other rigidbody involved in this collision</param>
	void OnOverlap(const CollisionManifold& manifold, RigidbodyComponent& other);

    void Update(float deltaTime);

	ScriptObject const * GetScriptObject();
};

