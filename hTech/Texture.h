#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include <string>

struct SDL_Texture;
class Transform;

class HTECH_FUNCTION_EXPORT Vector2;

class HTECH_FUNCTION_EXPORT Texture
{
protected:
	std::string m_Name;
	std::string m_Path;
	SDL_Texture* m_SDLTexture;

	//! Creates the sdl texture and returns the address that will be stored in the map.
	void Create(std::string texture_path, std::string name);

public:
	Texture(std::string texture_path, std::string name);
	~Texture();

	int Width;
	int Height;
	SDL_Texture& GetSDLTexture() const;
	const std::string& GetLocation() const;
	const std::string& GetName() const;
};