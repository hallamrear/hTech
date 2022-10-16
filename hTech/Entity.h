#pragma once
#include <string>
#include "Transform.h"

class  Collider;
class  Texture;
struct SDL_Renderer;
class Component;
class TransformComponent;

class Entity
{
private:

protected:
	std::vector<Component*> mComponents;

	bool					mIsWaitingToBeDestroyed;
	SDL_Renderer&			mRenderer;
	Texture*				mTexture;
	bool					mIsAlive;

	const SDL_Renderer&		GetRendererReference();
	void					ClampRotation();

public:
	std::string				Name;
	bool					IsEnabled;

	Entity(
		std::string texture_path = "");

	virtual					~Entity() = 0;

	//Sets mTexture to the texture at texture_path. Sets the width and height to the parameters.
	void					AssignTexture(const std::string& texture_path);
	const Texture&			GetTexture() const;

	virtual void			Update(double deltaTime) = 0;
	virtual void			Render() = 0;

	bool const				GetIsAlive()		 const { return mIsAlive; }
	
	virtual void			SetAlive(const bool state) { mIsAlive = state; }
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
		mComponents.push_back(new C());
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