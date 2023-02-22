#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include <string>
#include "Transform.h"
#include "Component.h"
#include "UI.h"

enum class ENUM_COMPONENT_LIST;
class HTECH_FUNCTION_EXPORT Collider;
class HTECH_FUNCTION_EXPORT Texture;
class HTECH_FUNCTION_EXPORT TransformComponent;

class HTECH_FUNCTION_EXPORT Entity
{
private:

protected:
	std::string m_Name;
	std::vector<Component*> m_Components;

	bool m_IsEnabled;
	bool m_IsWaitingToBeDestroyed;
	bool m_IsAlive;
	void ClampRotation();

public:
	const std::string& GetName() const;
	void SetName(const std::string& name);

	Entity(Transform SpawnTransform = Transform(), std::string Name = "unnamed", Entity* Parent = nullptr);
	virtual ~Entity();

	virtual void Update(float DeltaTime);
	virtual void Render(IRenderer& renderer);

	void RenderProperties();

	const bool GetIsEnabled() const { return m_IsEnabled; }
	void SetEnabled(const bool state);
	bool const GetIsAlive() const { return m_IsAlive; }
	virtual void SetAlive(const bool state) { m_IsAlive = state; }

	Entity* GetParent();
	void SetParent(Entity* entity);

	bool GetIsBeingDestroyed() const;
	void Destroy();

	template<typename T>
	T* GetComponent();
	template<class C>
	Entity* AddComponent();
	template<class C>
	Entity* RemoveComponent();

	Transform& GetTransform();

	void Serialize(Serializer& writer) const;
	void Deserialize(SerializedValue& serializedEntity);
};

template<class C>
inline C* Entity::GetComponent()
{
	C* ptr = nullptr;

	for (size_t i = 0; i < m_Components.size(); i++)
	{
		ptr = dynamic_cast<C*>(m_Components[i]);
		if (ptr != nullptr)
			return ptr;
	}

	return nullptr;
}

template<class C>
inline Entity* Entity::AddComponent()
{
	bool toAdd = true;
	for (size_t i = 0; i < m_Components.size(); i++)
	{
		//if we cast component to a type and it exists we cant add another one so we break the loop
		if (dynamic_cast<C*>(m_Components[i]) != nullptr)
		{
			toAdd = false;
		}
	}

	if (toAdd == true)
	{
		m_Components.push_back(new C(*this));
	}

	return this;
}

template<class C>
inline Entity* Entity::RemoveComponent()
{
	int pos = -1;

	for (size_t i = 0; i < m_Components.size(); i++)
	{
		if (dynamic_cast<C*>(m_Components[i]) != nullptr)
		{
			pos = i;
			break;
		}
	}

	if (pos != -1)
	{
		m_Components.erase(m_Components.begin() + pos);
	}

	return this;
}