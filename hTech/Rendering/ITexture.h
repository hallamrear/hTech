#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

struct SDL_Texture;
class Transform;

class HTECH_FUNCTION_EXPORT Vector2;

class HTECH_FUNCTION_EXPORT ITexture
{
protected:
	std::string m_Name;
	std::string m_Path;

	int m_Width;
	int m_Height;

	//! Creates the sdl texture and returns the address that will be stored in the map.
	virtual bool Create(const std::string& texture_path, const std::string& name) = 0;
	virtual bool Destroy() = 0;
	ITexture(const std::string& texture_path, const std::string& name);

public:
	~ITexture();

	const std::string& GetLocation() const;
	const std::string& GetName() const;

	const int& GetWidth() const;
	const int& GetHeight() const;
};