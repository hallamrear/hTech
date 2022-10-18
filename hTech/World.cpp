#include "pch.h"
#include "World.h"
#include "Entity.h"
#include "PhysicsWorld.h"
#include "InputManager.h"
#include "TextElement.h"
#include "Camera.h"
#include "Component_Rigidbody.h"
#include "Component_Sprite.h"
#include "Component_Animation.h"

World* World::mInstance = nullptr;

Entity* World::CreateEntity_Impl()
{
    Entity* entity = new Entity();
    mEntityList.push_back(entity);
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

void World::Update_Impl(double deltaTime)
{
    if (mEntityList.size() > 0)
    {
        mEntityList[mEntityList.size() - 1]->GetTransform().Position = InputManager::Get()->GetMouseWorldPosition();
    }

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

    Settings::Get()->SetDrawColliders(true);

    InputManager::Bind(
        IM_KEY_CODE::IM_KEY_1,
        IM_KEY_STATE::IM_KEY_PRESSED,
        [this]() 
        { 
            Entity* entity = CreateEntity_Impl();
            entity->AddComponent<AnimationComponent>();

            AnimationComponent* anim = entity->GetComponent<AnimationComponent>();
            anim->LoadAnimationSheet("Assets/test_animation.png");
            anim->IsLooping = true;
            anim->SetDuration(1.0f);
            anim->SetAnimationFrameCount(10);
            anim->SetAnimationCount(1);
        });

    InputManager::Bind(IM_KEY_CODE::IM_KEY_2, IM_KEY_STATE::IM_KEY_PRESSED, [this]() { Vector2 position = InputManager::Get()->GetMouseWorldPosition(); Entity* entity = CreateEntity_Impl(); entity->GetTransform().Position = position; entity->AddComponent<RigidbodyComponent>()->GetComponent<RigidbodyComponent>()->SetCollider(COLLIDER_TYPE::COLLIDER_AABB); entity->GetComponent<RigidbodyComponent>()->SetGravityEnabled(false); });
    InputManager::Bind(IM_KEY_CODE::IM_KEY_3, IM_KEY_STATE::IM_KEY_PRESSED, [this]() { Vector2 position = InputManager::Get()->GetMouseWorldPosition(); Entity* entity = CreateEntity_Impl(); entity->GetTransform().Position = position; entity->AddComponent<RigidbodyComponent>()->GetComponent<RigidbodyComponent>()->SetCollider(COLLIDER_TYPE::COLLIDER_OBB); entity->GetComponent<RigidbodyComponent>()->SetGravityEnabled(false); });
    InputManager::Bind(IM_KEY_CODE::IM_KEY_4, IM_KEY_STATE::IM_KEY_PRESSED, [this]() { Vector2 position = InputManager::Get()->GetMouseWorldPosition(); Entity* entity = CreateEntity_Impl(); entity->GetTransform().Position = position; entity->AddComponent<RigidbodyComponent>()->GetComponent<RigidbodyComponent>()->SetCollider(COLLIDER_TYPE::COLLIDER_SPHERE); entity->GetComponent<RigidbodyComponent>()->SetGravityEnabled(false); });

    InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_SCROLL_UP,   IM_KEY_STATE::IM_KEY_PRESSED, [this]() { if (mEntityList.back()->GetComponent<RigidbodyComponent>()->GetCollider()->mType == COLLIDER_TYPE::COLLIDER_OBB) mEntityList.back()->GetTransform().Rotation++; });
    InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_SCROLL_DOWN, IM_KEY_STATE::IM_KEY_PRESSED, [this]() { if (mEntityList.back()->GetComponent<RigidbodyComponent>()->GetCollider()->mType == COLLIDER_TYPE::COLLIDER_OBB) mEntityList.back()->GetTransform().Rotation--; });

    InputManager::Bind(
        IM_KEY_CODE::IM_KEY_0,
        IM_KEY_STATE::IM_KEY_PRESSED,
        [this]()
        { 
            if (mEntityList.size() != 0)
            {
                DestroyEntity_Impl(mEntityList.back());
            }
        });
}

World::~World()
{
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

Entity* World::CreateEntity()
{
    return Get()->CreateEntity_Impl();
}

void World::DestroyEntity(Entity* entity)
{
    Get()->DestroyEntity_Impl(entity);
}

void World::Update(double deltaTime)
{
    Get()->Update_Impl(deltaTime);
}

void World::Render(SDL_Renderer& renderer)
{
    Get()->Render_Impl(renderer);
}
