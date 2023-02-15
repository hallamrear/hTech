#include "pch.h"
#include "World.h"
#include "Entity.h"
#include "PhysicsWorld.h"
#include "InputManager.h"
#include "Text.h"
#include "Camera.h"
#include "Transform.h"
#include "Editor.h"
#include "Console.h"

#include "UI.h"

#include "Component_Sprite.h"
#include "Component_Script.h"

World* World::m_Instance = nullptr;

void World::UpdateHashmapNames_Impl()
{
    std::vector<std::string> toErase = std::vector<std::string>();
    size_t size = m_EntityMap.size();
    for (auto& itr : m_EntityMap)
    {
        if (itr.first != itr.second->GetName())
        {
            Entity* entity = itr.second;
            m_EntityMap.insert(std::make_pair(itr.second->GetName().c_str(), itr.second));
            toErase.push_back(itr.first);
        }
    }

    for (size_t i = 0; i < toErase.size(); i++)
    {
        m_EntityMap.erase(toErase[i]);
    }
}

Entity* World::CreateEntity_Impl(std::string Name, Transform SpawnTransform, Entity* Parent)
{
    Entity* entity = new Entity(SpawnTransform, Name, Parent);
    m_EntityMap.insert(std::make_pair(entity->GetName(), entity));
    
    if (m_WorldHashMap)
    {
        m_WorldHashMap->Insert(entity);
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
        m_EntityMap.erase(itr);
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
    m_WorldHashMap->Clear();
    m_WorldHashMap->Update(DeltaTime);

    for (auto& itr : m_EntityMap)
    {
        if (itr.second != nullptr)
        {
            if (itr.second->GetIsEnabled())
            {
                itr.second->Update(DeltaTime);
            }

            m_WorldHashMap->Insert(itr.second);
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
        m_WorldHashMap->Render(renderer);
    }

    for(auto& itr : m_EntityMap)
    {
        if (itr.second != nullptr)
        {
            std::string str = itr.second->GetName();

            if (str == "")
            {
                str += "unnamed###" + itr.second->GetIsAlive() + std::to_string(itr.second->GetTransform().Position.X) + "/" + std::to_string(itr.second->GetTransform().Position.Y);
            }

            if (ImGui::Selectable(str.c_str()))
            {
                Editor::SetSelectedEntity(itr.second);
            }
            
            itr.second->Render();
        }
    }

    ImGui::End();
}
#else
void World::Render_Impl(SDL_Renderer& renderer)
{
    for (auto& entity : m_EntityMap)
    {
        if (entity.second != nullptr)
        {
            entity.second->Render();
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
    m_WorldHashMap->Retrieve(rect, entities);
}

World::World()
{
    int sizeX = WORLD_TILE_COUNT_X;
    int sizeY = WORLD_TILE_COUNT_Y;
    sizeX -= (sizeX % 2);
    sizeY -= (sizeY% 2);

    int halfSizeX = WORLD_TILE_COUNT_X / 2;
    int halfSizeY = WORLD_TILE_COUNT_Y / 2;

    m_WorldHashMap = new SpatialHash(sizeX, sizeY);
}

World::~World()
{
    if (m_WorldHashMap)
    {
        delete m_WorldHashMap;
        m_WorldHashMap = nullptr;
    }

    for (auto& itr : m_EntityMap)
    {
        DestroyEntity_Impl(itr.second);
    }

    m_EntityMap.clear();
}

World* World::Get()
{
    if (m_Instance == nullptr)
        m_Instance = new World();

    return m_Instance;
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

void World::ResetWorldEntities()
{
    return Get()->ResetWorldEntities_Impl();
}

void World::CallStartFunctionOnAllEntites()
{
    return Get()->CallStartFunctionOnAllEntites_Impl();
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
    m_WorldHashMap->Retrieve(rect, entities);
    
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

void World::UpdateHashmapNames()
{
    return Get()->UpdateHashmapNames_Impl();
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

void World::ResetWorldEntities_Impl()
{
    for (auto& itr : m_EntityMap)
    {
        ScriptComponent* script = itr.second->GetComponent<ScriptComponent>();

        if (script != nullptr)
        {
            script->Reset();
        }
    }
}

void World::CallStartFunctionOnAllEntites_Impl()
{
    for (auto& itr : m_EntityMap)
    {
        ScriptComponent* script = itr.second->GetComponent<ScriptComponent>();

        if (script != nullptr)
        {
            script->Start();
        }
    }
}

void World::Serialize(Serializer& writer)
{
    Get()->Serialize_Impl(writer);
}

void World::Deserialize(Deserializer& reader)
{
    return Get()->Deserialize_Impl(reader);
}