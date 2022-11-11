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

World* World::mInstance = nullptr;

Entity* World::CreateEntity_Impl(std::string Name, Transform SpawnTransform, Entity* Parent)
{
    Entity* entity = new Entity(SpawnTransform, Name, Parent);
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
    mWorldHashMap->Clear();
    mWorldHashMap->Update(DeltaTime);

    for (size_t i = 0; i < mEntityList.size(); i++)
    {
        if (mEntityList[i] != nullptr)
        {
            mEntityList[i]->Update(DeltaTime);
            mWorldHashMap->Insert(mEntityList[i]);
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


    for (size_t i = 0; i < mSelectedEntities.size(); i++)
    {
        Vector2 rectTR = Camera::WorldToScreen(mSelectedEntities[i]->GetTransform().Position + Vector2(-32, 32));
        SDL_Rect rect{};
        rect.x = rectTR.X;
        rect.y = rectTR.Y;
        rect.w = 64;
        rect.h = 64;

        SDL_RenderDrawRect(&renderer, &rect);
    }

    Vector2 points[4] =
    {
        {(float)selectionRect.X,				   (float)selectionRect.Y},
        {(float)selectionRect.X + selectionRect.W, (float)selectionRect.Y},
        {(float)selectionRect.X,				   (float)selectionRect.Y - selectionRect.H},
        {(float)selectionRect.X + selectionRect.W, (float)selectionRect.Y - selectionRect.H}
    };

    SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
    Vector2 r;
    SDL_Rect c{};
    for (size_t i = 0; i < 4; i++)
    {
        r = Camera::WorldToScreen(points[i]);
        c.w = 4;
        c.h = 4;
        c.x = r.X - (c.w / 2);
        c.y = r.Y - (c.h / 2);
        SDL_RenderFillRect(&renderer, &c);
    }

    if (mIsDraggingRect)
    {
        SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
        selectionRect.Render(renderer);
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

    UI::CreateVariableTracker(UI_Panel(1, 2, 6, 1), selectionRect.X, "1 X: ");
    UI::CreateVariableTracker(UI_Panel(1, 3, 6, 1), selectionRect.Y, "1 Y: ");
    UI::CreateVariableTracker(UI_Panel(1, 4, 6, 1), selectionRect.W, "1 W: ");
    UI::CreateVariableTracker(UI_Panel(1, 5, 6, 1), selectionRect.H, "1 H: ");

    mWorldHashMap = new SpatialHash(sizeX, sizeY);
    mSelectedEntities = std::vector<Entity*>();

    InputManager::Bind(
        IM_KEY_CODE::IM_KEY_1,
        IM_KEY_STATE::IM_KEY_PRESSED,
        [this]()
        {
            Entity* entity = CreateEntity_Impl("Test", Transform(InputManager::Get()->GetMouseWorldPosition()));
            entity->AddComponent<SpriteComponent>();
            entity->GetComponent<SpriteComponent>()->LoadTexture("Assets/test.png");
        });


    InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_PRESSED, [this]()
        {
            rectStart = InputManager::Get()->GetMouseWorldPosition();
            mIsDraggingRect = true;
        });

    InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_HELD, [this]()
        {
            Vector2 mousePos = InputManager::Get()->GetMouseWorldPosition();
            selectionRect = WorldRectangle(rectStart, mousePos);
        });

    InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_RELEASED, [this]()
        {
            rectEnd = InputManager::Get()->GetMouseWorldPosition();
            selectionRect = WorldRectangle(rectStart, rectEnd);       
            mWorldHashMap->Retrieve(selectionRect, mSelectedEntities);
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
