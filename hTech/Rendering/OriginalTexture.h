#pragma once
#include "Rendering/ITexture.h"

class HTECH_FUNCTION_EXPORT OriginalTexture :
    public ITexture
{
private:
    SDL_Texture* m_SDLTexture;

protected:
    bool Create(const std::string& texture_path, const std::string& name);
    bool Destroy();

public:
    OriginalTexture(const std::string& texture_path, const std::string& name);
    ~OriginalTexture();

    SDL_Texture& GetSDLTexture() const;
};

