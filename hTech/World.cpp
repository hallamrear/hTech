#include "pch.h"
#include "World.h"
#include "Entity.h"
#include "PhysicsWorld.h"
#include "Rigidbody.h"
#include "InputManager.h"
#include "TextElement.h"
#include "Camera.h"

World* World::mInstance = nullptr;

///
/// TODO: When reimplementing, make sure to search for every instance of this because some functions calls are commented out
/// 

//Entity& World::CreateEntity_Impl()
//{
//    //IMPLEMENT
//}
//
//void World::DestroyEntity_Impl(Entity* entity)
//{
//    //IMPLEMENT
//}

Rigidbody& World::CreateRigidbody_Impl()
{
    //IMPLEMENT
    Rigidbody* rb = new Rigidbody("Assets/cat.png", PhysicsProperties());
    mEntityList.push_back(rb);
    return *rb;
}

void World::DestroyRigidbody_Impl(Rigidbody* entity)
{
    if (entity == nullptr)
        return;

    auto itr = std::find(mEntityList.begin(), mEntityList.end(), entity);

    if (itr != mEntityList.end())
    {
        Entity* entity = *itr._Ptr;
        entity->Destroy();
        entity = nullptr;
    }
}

void World::ClearupEntities()
{
    if (mEntityList.size() != 0)
    {
        for (std::vector<Entity*>::iterator it = mEntityList.begin(); it != mEntityList.end(); /*it++*/)
        {
            auto i = *it;
            if (i == nullptr)
            {
                ++it;
            }
            else
            {
                if (i->GetIsBeingDestroyed() == true)
                {
                    it = mEntityList.erase(it);
                }
                else
                {
                    ++it;
                }
            }            
        }
    }
}

void World::Update_Impl(double deltaTime)
{
    for (size_t i = 0; i < mEntityList.size(); i++)
    {
        if (mEntityList[i] != nullptr)
        {
            mEntityList[i]->Update(deltaTime);
        }

        if (mSceneGraphText != nullptr)
        {
            mSceneGraphText->Update(deltaTime);
        }
    }

    ClearupEntities();
}

void World::Render_Impl(SDL_Renderer& renderer)
{
    float offset = 10;

    for (size_t i = 0; i < mEntityList.size(); i++)
    {
        if (mEntityList[i] != nullptr)
        {
            mEntityList[i]->Render();
        }

        if (mSceneGraphText != nullptr)
        {
            offset += 20;
            Vector2 pos = Camera::ScreenToWorld(Vector2(64.0f, offset));
            std::string str = "test: " + std::to_string(mEntityList[i]->GetTransform().Position.Y);
            mSceneGraphText->SetPosition(pos);
            mSceneGraphText->SetString(str);
            mSceneGraphText->Update(0.0f);
            mSceneGraphText->Render();
        }
    }
}

World::World()
{
    mSceneGraphText = new TextElement("UNSET TEXT STRING");
    InputManager::Bind(IM_KEY_CODE::IM_KEY_1, IM_KEY_STATE::IM_KEY_PRESSED, [this]() { CreateRigidbody_Impl(); });
    InputManager::Bind(IM_KEY_CODE::IM_KEY_2, IM_KEY_STATE::IM_KEY_PRESSED, [this]() { if(mEntityList.size() != 0) DestroyRigidbody_Impl((Rigidbody*)mEntityList.back()); });
}

World::~World()
{
    for (size_t i = 0; i < mEntityList.size(); i++)
    {
        Rigidbody* rb = dynamic_cast<Rigidbody*>(mEntityList[i]);

        if (rb != nullptr)
        {
            Physics::DeregisterRigidbody(rb);
            DestroyRigidbody_Impl(rb);
        }
        else
        {
            //DestroyEntity_Impl(mEntityList[i]);
        }
    }

    mEntityList.clear();
}

World* World::Get()
{
    if (mInstance == nullptr)
        mInstance = new World();

    return mInstance;
}

//Entity& World::CreateEntity()
//{
//    return Get()->CreateEntity_Impl();
//}

//void World::DestroyEntity(Entity& entity)
//{
//    Get()->DestroyEntity_Impl(&entity);
//}

Rigidbody& World::CreateRigidbody()
{
    return Get()->CreateRigidbody_Impl();
}

void World::DestroyRigidbody(Rigidbody& entity)
{
    Get()->DestroyRigidbody_Impl(&entity);
}

void World::Update(double deltaTime)
{
    Get()->Update_Impl(deltaTime);
}

void World::Render(SDL_Renderer& renderer)
{
    Get()->Render_Impl(renderer);
}
