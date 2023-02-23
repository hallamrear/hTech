#pragma once
#include "IRenderer.h"

struct SDL_Renderer;
class OriginalWindow;

class OriginalRenderer : public IRenderer
{
private:
	SDL_Renderer* m_SDLRenderer;
	SDL_Texture* m_RenderToTextureTarget;

	void CreateRenderTargetTexture(const Vector2& size);
	void SettingDearIMGUIColourScheme();
	void InitialiseDearIMGUI(IWindow& window);

public:
	OriginalRenderer();
	~OriginalRenderer();

	void Startup(const IWindow& window) override;
	void Shutdown() override;

	/// <summary>
	/// Creates a screenshot of the current frame.
	/// </summary>
	/// <param name="name">Name of the output file.</param>
	void TakeScreenshot(const std::string& name);

	SDL_Renderer* GetAPIRenderer() const;

	void EndFrame();
	void StartFrame();

	void SetClearColour(const Colour& colour);
	void SetPrimativeDrawColour(const Colour& colour);
	
	void Render_Point(const Vector2& worldSpacePoint);
	void Render_WorldSpaceLine(const Vector2&  pointA, const Vector2& pointB);
	void Render_ScreenSpaceLine(const Vector2& pointA, const Vector2& pointB);
	void Render_WorldSpaceRectangle(const WorldRectangle& rectangle, const RENDER_LAYER& layer = RENDER_LAYER::DEFAULT, bool filled = true);
	void Render_ScreenSpaceRectangle(const ScreenRectangle& rectangle, const RENDER_LAYER& layer = RENDER_LAYER::DEFAULT, bool filled = true);

	void Render_Texture(const Texture& texture, const Transform& transform, const RENDER_LAYER& layer = RENDER_LAYER::DEFAULT, const bool& flipped = false);	
	void Render_Texture(const Texture& texture, const Transform& transform, const RENDER_LAYER& layer = RENDER_LAYER::DEFAULT, const Vector2* center = nullptr, const WorldRectangle* srcRect = nullptr, const WorldRectangle* dstRect = nullptr, const bool& flipped = false);


	/// <summary>
	/// 
	/// </summary>
	/// <param name="texture"></param>
	void SetRenderTexture(SDL_Texture* texture);
	SDL_Texture* GetRenderTexture();
};

