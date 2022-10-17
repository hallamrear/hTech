#pragma once
#include "Component.h"

class Texture;

class SpriteComponent :
    public Component
{
private:
    Texture* mTexture;
    bool     mIsFlipped;

protected:

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

