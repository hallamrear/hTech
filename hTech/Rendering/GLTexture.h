#pragma once
#include <Rendering/ITexture.h>

class HTECH_FUNCTION_EXPORT GLTexture :
    public ITexture
{
private:
    unsigned int m_TextureID;

protected:
    bool Create(const std::string& texture_path, const std::string& name);
    bool Destroy();

public:
    GLTexture(const std::string& texture_path, const std::string& name);
    ~GLTexture();

    const unsigned int& GetID() const;
};

