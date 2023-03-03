#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "Component.h"
#include "ScriptLoader.h"
#include "Observer.h"

class ScriptObject;
class CollisionManifold;
class RigidbodyComponent;

class HTECH_FUNCTION_EXPORT ScriptComponent :
    public Component, public Observer
{
private:
	std::string m_ScriptReferenceName;
    ScriptObject* m_ScriptObject; 
    
protected:
    virtual void Destroy();
	void RenderProperties();

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
	/// Called every time the stop button is pressed to reset the entity.
	/// This should be used to set any starting parameters for the entity.
	/// </summary>
	void Reset();

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

	void Serialize(Serializer& writer) const override;
	void Deserialize(SerializedValue& value) override;
	void OnNotify() override;

	void SetClass(const std::string& name);
};

