#pragma once
#include "Rendering/ITexture.h"

struct SDL_Texture;

class HTECH_FUNCTION_EXPORT OriginalTexture :
    public ITexture
{
private:
    friend class OriginalRenderer;
    SDL_Texture* m_SDLTexture;

protected:
    bool Create(const unsigned int& width, const unsigned int& height) override;
    bool Load(const std::string& texture_path, const std::string& name) override;
    bool Destroy() override;

    OriginalTexture();

public:
    ~OriginalTexture();

    SDL_Texture& GetSDLTexture() const;
};

