#include "pch.h"
#include "GLShader.h"
#include <System/Console.h>

GLShader::GLShader()
{
    m_ID = GL_INVALID_VALUE;
}

GLShader::~GLShader()
{
    Destroy();
}

GLuint GLShader::CreateShader(const char* data, SHADER_TYPE type)
{
    unsigned int shader = 0;
    int success;
    
    switch (type)
    {
    case IShader::SHADER_TYPE::VERTEX:
        shader = glCreateShader(GL_VERTEX_SHADER);
        break;
    case IShader::SHADER_TYPE::PIXEL:     
        shader = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    default:
        return GL_INVALID_VALUE;
        break;
    }
    glShaderSource(shader, 1, &data, NULL);
    glCompileShader(shader);
    // print compile errors if any
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, NULL, buffer);
        std::string str(buffer);
        Console::LogMessage(LogLevel::LOG_ERROR, str);
        return GL_INVALID_VALUE;
    };

    return shader;
}

bool GLShader::Create(const char* pixelShaderSource, const char* vertexShaderSource)
{
    int success;
    GLuint vertex = CreateShader(vertexShaderSource, SHADER_TYPE::VERTEX);
    if (vertex == GL_INVALID_VALUE)
    {
        char buffer[512];
        glGetShaderInfoLog(m_ID, 512, NULL, buffer);
        std::string str(buffer);
        Console::LogMessage(LogLevel::LOG_ERROR, str);
        return false;
    }

    GLuint pixel = CreateShader(pixelShaderSource, SHADER_TYPE::PIXEL);
    if (pixel == GL_INVALID_VALUE)
    {
        char buffer[512];
        glGetShaderInfoLog(m_ID, 512, NULL, buffer);
        std::string str(buffer);
        Console::LogMessage(LogLevel::LOG_ERROR, str);
        return false;
    }

    // shader Program
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, pixel);
    glLinkProgram(m_ID);
    // print linking errors if any
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char buffer[512];
        glGetProgramInfoLog(m_ID, 512, NULL, buffer);
        std::string str(buffer);
        Console::LogMessage(LogLevel::LOG_ERROR, str);
        Destroy();
        return false;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(pixel);

    return true;
}

void GLShader::Destroy()
{
    glDeleteProgram(m_ID);
    m_ID = GL_INVALID_VALUE;
}

void GLShader::Bind() const
{
    glUseProgram(m_ID);
}

void GLShader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void GLShader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void GLShader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}