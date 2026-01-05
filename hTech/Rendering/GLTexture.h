#pragma once
#include <Rendering/ITexture.h>

class HTECH_FUNCTION_EXPORT GLTexture :
    public ITexture
{
private:
    friend class OpenGLRenderer;
    unsigned int m_TextureID;

protected:
    bool Create(const unsigned int& width, const unsigned int& height) override;
    bool Load(const std::string& texture_path, const std::string& name) override;
    bool Destroy() override;

    GLTexture();

public:
    ~GLTexture();

    const unsigned int& GetID() const;
};

