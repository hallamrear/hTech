#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "Component.h"
#include "Texture.h"

class HTECH_FUNCTION_EXPORT SpriteComponent :
    public Component
{
private:
    Texture* m_SDLTexture;
    bool     m_IsFlipped;

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

    void Serialize(Serializer& writer) const override;
    void Deserialize(SerializedValue& value) override;
};