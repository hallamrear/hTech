#pragma once
#include "IShader.h"
#include "External/OPENGL.h"

class GLShader :
    public IShader
{
private:
    GLuint CreateShader(const char* data, SHADER_TYPE type);

public:
    GLShader();
    ~GLShader();

    bool Create(const char* pixelShaderSource, const char* vertexShaderSource) override;
    void Destroy() override;
    void Bind() const override;
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
};