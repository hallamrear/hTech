#include "pch.h"
#include "Rendering/GLTexture.h"
#include <External/GLEW/include/GL/glew.h>
#include <System/Console.h>

#include <External/STB/include/stb_image.h>

GLTexture::GLTexture(const std::string& texture_path, const std::string& name)
	: ITexture(texture_path, name)
{
	m_TextureID = 0;
	Create(texture_path, name);
}

GLTexture::~GLTexture()
{

}

const unsigned int& GLTexture::GetID() const
{
	return m_TextureID;
}

bool GLTexture::Create(const std::string& texture_path, const std::string& name)
{
	if (texture_path == "")
	{
		return false;
	}

	//Generate a texture ID.
	glGenTextures(1, &m_TextureID);

	//Bind it to edit.
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	int nrChannels;
	unsigned char* data = stbi_load(texture_path.c_str(), &m_Width, &m_Height, &nrChannels, 0);

	if (data)
	{
		//Create image object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		// set wrapping/filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		Console::LogMessage(LogLevel::LOG_ERROR, std::string("Failed to load image <" + texture_path + "> \n"));
		stbi_image_free(data);
		return false;
	}

	stbi_image_free(data);

	return true;
}

bool GLTexture::Destroy()
{
	glDeleteTextures(1, &m_TextureID);
	return true;
}