#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



struct SDL_Renderer;

class HTECH_FUNCTION_EXPORT [[deprecated("Use World instead")]] GameState
{
public:
	[[deprecated("Use World instead")]]
	virtual ~GameState() = default;
	[[deprecated("Use World instead")]]
	virtual void Start() = 0;
	[[deprecated("Use World instead")]]
	virtual void Update(double) = 0;
	[[deprecated("Use World instead")]]
	virtual void Render(SDL_Renderer&) = 0;
	[[deprecated("Use World instead")]]
	virtual void End() = 0;
};