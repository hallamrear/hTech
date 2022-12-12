#include "pch.h"
#include "World.h"
#include "Entity.h"
#include "PhysicsWorld.h"
#include "InputManager.h"
#include "Text.h"
#include "Camera.h"
#include "Transform.h"

#include "UI.h"

#include "Component_Sprite.h"
#include "Component_Script.h"

World* World::mInstance = nullptr;

Entity* World::CreateEntity_Impl(std::string Name, Transform SpawnTransform, Entity* Parent)
{
    Entity* entity = new Entity(SpawnTransform, Name, Parent);
    m_EntityMap.insert(std::make_pair(Name, entity));
    
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

    auto itr = m_EntityMap.find(entity->GetName());

    if (itr != m_EntityMap.end())
    {
        itr->second->Destroy();
    }
}

void World::ClearupDeadEntities()
{
    if (m_EntityMap.size() != 0)
    {
        for (std::unordered_map<std::string, Entity*>::iterator itr = m_EntityMap.begin(); itr != m_EntityMap.end(); /*it++*/)
        {
            Entity* entity = itr->second;
            if (entity == nullptr)
            {
                ++itr;
            }
            else
            {
                if (entity->GetIsBeingDestroyed() == true)
                {
                    delete itr->second;
                    itr->second = nullptr;
                    itr = m_EntityMap.erase(itr);
                }
                else
                {
                    ++itr;
                }
            }            
        }
    }
}

void World::Update_Impl(float DeltaTime)
{
    mWorldHashMap->Clear();
    mWorldHashMap->Update(DeltaTime);

    for (auto& itr : m_EntityMap)
    {
        if (itr.second != nullptr)
        {
            if (itr.second->IsEnabled)
            {
                itr.second->Update(DeltaTime);
            }

            mWorldHashMap->Insert(itr.second);
        }
    }

    ClearupDeadEntities();
}

#ifdef _DEBUG
void World::Render_Impl(SDL_Renderer& renderer)
{
    ImGui::Begin("Scene Graph");

    if (Console::Query("DrawHashMap") != 0)
    {
        mWorldHashMap->Render(renderer);
    }

    for(auto& itr : m_EntityMap)
    {
        if (itr.second != nullptr)
        {
            ImGui::Text(itr.second->GetName().c_str());
            itr.second->Render();
        }
    }

    ImGui::End();
}
#else
void World::Render_Impl(SDL_Renderer& renderer)
{
    for (size_t i = 0; i < m_EntityMap.size(); i++)
    {
        if (m_EntityMap[i] != nullptr)
        {
            m_EntityMap[i]->Render();
        }
    }
}
#endif

Entity* World::GetEntityByName_Impl(std::string name)
{
    Entity* entity = nullptr;
    auto result = m_EntityMap.find(name);

    if (result != m_EntityMap.end())
    {
        return result->second;
    }

    return nullptr;
}

void World::QuerySpaceForEntities_Impl(WorldRectangle rect, std::vector<Entity*>& entities)
{
    mWorldHashMap->Retrieve(rect, entities);
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
        IM_KEY_CODE::IM_KEY_A,
        IM_KEY_STATE::IM_KEY_PRESSED,
        [this]()
        {
            Entity* entity = CreateEntity_Impl("Test" + std::to_string(rand() % 100), Transform(InputManager::Get()->GetMouseWorldPosition()));
            entity->AddComponent<SpriteComponent>();
            entity->GetComponent<SpriteComponent>()->LoadTexture("test.png");
            entity->AddComponent<ScriptComponent>();
        });   
}

World::~World()
{
    if (mWorldHashMap)
    {
        delete mWorldHashMap;
        mWorldHashMap = nullptr;
    }

    for (auto& itr : m_EntityMap)
    {
        DestroyEntity_Impl(itr.second);
    }

    m_EntityMap.clear();
}

World* World::Get()
{
    if (mInstance == nullptr)
        mInstance = new World();

    return mInstance;
}

void World::QuerySpaceForEntities(WorldRectangle rect, std::vector<Entity*>& entities)
{
    Get()->QuerySpaceForEntities_Impl(rect, entities);
}

Entity* World::CreateEntity(std::string Name, Transform SpawnTransform, Entity* Parent)
{
    return Get()->CreateEntity_Impl(Name, SpawnTransform, Parent);
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

void World::UnloadAll()
{
    return Get()->ClearAllEntities();
}

Entity* World::FindNearestEntityToPosition_Impl(Vector2 WorldPosition)
{
    Entity* closestEntity = nullptr;
    std::vector<Entity*> entities = std::vector<Entity*>();
    int size = 64;
    WorldRectangle rect = WorldRectangle((int)WorldPosition.X - (size / 2), (int)WorldPosition.Y + (size / 2), size, size);
    float minDistance = FLT_MAX;
    rect = WorldRectangle((int)WorldPosition.X - (size / 2), (int)WorldPosition.Y + (size / 2), size, size);
    mWorldHashMap->Retrieve(rect, entities);
    
    for (std::vector<Entity*>::iterator itr = entities.begin(); itr != entities.end(); itr++)
    {
        Entity* ent = *itr;
        Vector2 position = ent->GetTransform().Position;

        Vector2 diff = Vector2(WorldPosition.X - position.X, WorldPosition.Y - position.Y);
        float mag = diff.GetMagnitude();
        if (mag < minDistance)
        {
            minDistance = mag;
            closestEntity = *itr;
        }
    }

    if (closestEntity)
        return closestEntity;
    else
        return nullptr;
}

void World::ClearAllEntities()
{
    //todo : proper cleanup
    /*
    for (auto& itr : m_EntityMap)
    {
        m_EntityMap.erase(itr.first);
    }*/

    m_EntityMap.clear();
}

Entity* World::FindNearestEntityToPosition(Vector2 WorldPosition)
{
    return Get()->FindNearestEntityToPosition_Impl(WorldPosition);
}

void World::Serialize_Impl(Serializer& writer) const
{
    writer.String("Entities");
    writer.StartArray();
    for (auto& itr : m_EntityMap)
    {
        itr.second->Serialize(writer);
    }
    writer.EndArray();
}

void World::Deserialize_Impl(Deserializer& reader)
{
    auto value = reader.IsArray();
    SerializedValue results = reader["Entities"].GetArray();

    Entity* entity = nullptr;
    for (rapidjson::SizeType i = 0; i < results.Size(); i++)
    {
        if (results[i]["Name"].IsString())
        {
            entity = CreateEntity_Impl(results[i]["Name"].GetString());
            entity->Deserialize(results[i]);
        }
    }

    entity = nullptr;
}

void World::Serialize(Serializer& writer)
{
    Get()->Serialize_Impl(writer);
}

void World::Deserialize(Deserializer& reader)
{
    return Get()->Deserialize_Impl(reader);
}