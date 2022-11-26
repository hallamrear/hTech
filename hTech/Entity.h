#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include <string>
#include "Transform.h"

class HTECH_FUNCTION_EXPORT  Collider;
class HTECH_FUNCTION_EXPORT  Texture;
struct SDL_Renderer;
class HTECH_FUNCTION_EXPORT Component;
class HTECH_FUNCTION_EXPORT TransformComponent;

class HTECH_FUNCTION_EXPORT Entity
{
private:

protected:
	std::string				mName;
	std::vector<Component*> mComponents;

	bool					mIsWaitingToBeDestroyed;
	bool					mIsAlive;
	void					ClampRotation();

public:
	const std::string&      GetName() const;
	bool					IsEnabled;

							Entity(Transform SpawnTransform = Transform(), std::string Name = "unnamed", Entity* Parent = nullptr);
	virtual					~Entity();

	virtual void			Update(float DeltaTime);
	virtual void			Render();

	bool const				GetIsAlive()		 const { return mIsAlive; }
	virtual void			SetAlive(const bool state) { mIsAlive = state; }

	Entity*					GetParent();
	void					SetParent(Entity* entity);

	bool					GetIsBeingDestroyed() const;
	void					Destroy();


	template<typename T>
	T* GetComponent();
	template<class C>
	Entity* AddComponent();
	template<class C>
	Entity* RemoveComponent();

	Transform& GetTransform();
};

template<class C>
inline C* Entity::GetComponent()
{
	C* ptr = nullptr;

	for (size_t i = 0; i < mComponents.size(); i++)
	{
		ptr = dynamic_cast<C*>(mComponents[i]);
		if (ptr != nullptr)
			return ptr;
	}

	return nullptr;
}

template<class C>
inline Entity* Entity::AddComponent()
{
	bool toAdd = true;
	for (size_t i = 0; i < mComponents.size(); i++)
	{
		//if we cast component to a type and it exists we cant add another one so we break the loop
		if (dynamic_cast<C*>(mComponents[i]) != nullptr)
		{
			toAdd = false;
		}
	}

	if (toAdd == true)
	{
		mComponents.push_back(new C(*this));
	}

	return this;
}

template<class C>
inline Entity* Entity::RemoveComponent()
{
	int pos = -1;

	for (size_t i = 0; i < mComponents.size(); i++)
	{
		if (dynamic_cast<C*>(mComponents[i]) != nullptr)
		{
			pos = i;
			break;
		}
	}

	if (pos != -1)
	{
		mComponents.erase(mComponents.begin() + pos);
	}

	return this;
}