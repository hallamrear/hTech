#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include <string>

class Transform;

class HTECH_FUNCTION_EXPORT Vector2;
struct SDL_Texture;

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
	SDL_Texture& GetSDLTexture();
	const std::string& GetLocation() const;
	const std::string& GetName() const;

	virtual void Render(SDL_Renderer& renderer, const Transform& transform, const Vector2& sourcePosition, const Vector2& sourceDimensions, const bool& flipped);
	virtual void Render(SDL_Renderer& renderer, const Transform& transform, const Vector2& sourcePosition, const Vector2& sourceDimensions);
	virtual void Render(SDL_Renderer& renderer, const Transform& transform, const bool& flipped);
	virtual void Render(SDL_Renderer& renderer, const Transform& transform);
};