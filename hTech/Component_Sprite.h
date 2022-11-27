#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include "Component.h"

class HTECH_FUNCTION_EXPORT Texture;

class HTECH_FUNCTION_EXPORT SpriteComponent :
    public Component
{
private:
    Texture* mTexture;
    bool     mIsFlipped;

protected:
    void RenderProperties();

public:
    SpriteComponent(Entity& entity);
    ~SpriteComponent();

    void SetIsFlipped(bool state);
    bool IsFlipped();
    void LoadTexture(std::string texture_path);
    void UnloadTexture();

    void Update(float DeltaTime);
    void Render(SDL_Renderer& renderer);
};

