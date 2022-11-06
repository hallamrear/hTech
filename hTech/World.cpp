#include "pch.h"
#include "World.h"
#include "Entity.h"
#include "PhysicsWorld.h"
#include "InputManager.h"
#include "Text.h"
#include "Camera.h"

World* World::mInstance = nullptr;

Entity* World::CreateEntity_Impl(std::string name)
{
    Entity* entity = new Entity(name);
    mEntityList.push_back(entity);
    
    if (mWorldHashMap)
    {
        mWorldHashMap->Insert(entity);
    }

    return entity;
}

void World::DestroyEntity_Impl(Entity* entity)
{
    if (entity == nullptr)
        return;

    auto itr = std::find(mEntityList.begin(), mEntityList.end(), entity);

    if (itr != mEntityList.end())
    {
        Entity* entity = *itr._Ptr;
        entity->Destroy();
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
                    Entity* e = *it._Ptr;
                    delete e;
                    e = nullptr;
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

void World::Update_Impl(float DeltaTime)
{
    mWorldHashMap->Update(DeltaTime);

    for (size_t i = 0; i < mEntityList.size(); i++)
    {
        if (mEntityList[i] != nullptr)
        {
            mEntityList[i]->Update(DeltaTime);
        }
    }

    ClearupEntities();
}

void World::Render_Impl(SDL_Renderer& renderer)
{
    mWorldHashMap->Render(renderer);

    for (size_t i = 0; i < mEntityList.size(); i++)
    {
        if (mEntityList[i] != nullptr)
        {
            mEntityList[i]->Render();
        }
    }
}

Entity* World::GetEntityByName_Impl(std::string name)
{
    Entity* entity = nullptr;

    for (size_t i = 0; i < mEntityList.size(); i++)
    {
        if (mEntityList[i]->GetName() == name)
        {
            entity = mEntityList[i];
            break;
        }
    }

    return entity;
}

World::World()
{
    int sizeX = WORLD_TILE_COUNT_X;
    int sizeY = WORLD_TILE_COUNT_Y;
    sizeX -= (sizeX % 2);
    sizeY -= (sizeY% 2);

    int halfSizeX = WORLD_TILE_COUNT_X / 2;
    int halfSizeY = WORLD_TILE_COUNT_Y / 2;

    mWorldHashMap = new SpatialHash(sizeX, sizeY);

    InputManager::Bind(
        IM_KEY_CODE::IM_KEY_1,
        IM_KEY_STATE::IM_KEY_PRESSED,
        [this]()
        {
            Entity* entity = CreateEntity_Impl();
            entity->GetTransform().Position = InputManager::Get()->GetMouseWorldPosition();
        });
}

World::~World()
{
    if (mWorldHashMap)
    {
        delete mWorldHashMap;
        mWorldHashMap = nullptr;
    }

    for (size_t i = 0; i < mEntityList.size(); i++)
    {
        DestroyEntity_Impl(mEntityList[i]);
    }

    mEntityList.clear();
}

World* World::Get()
{
    if (mInstance == nullptr)
        mInstance = new World();

    return mInstance;
}

Entity* World::CreateEntity(std::string name)
{
    return Get()->CreateEntity_Impl();
}

void World::DestroyEntity(Entity* entity)
{
    Get()->DestroyEntity_Impl(entity);
}

Entity* World::GetEntityByName(std::string name)
{
    return Get()->GetEntityByName_Impl(name);
}

void World::Update(float DeltaTime)
{
    Get()->Update_Impl(DeltaTime);
}

void World::Render(SDL_Renderer& renderer)
{
    Get()->Render_Impl(renderer);
}
