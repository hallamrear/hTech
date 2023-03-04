#pragma once
#include "RenderLayer.h"
#include "Colour.h"
#include "Rectangle.h"
#include "Transform.h"

class Texture;
class IWindow;
class RenderTexture;

class IRenderer
{

private:

protected:
	bool m_IsInitialised;
	RenderLayer m_Layers[(int)RENDER_LAYER::COUNT];
	Colour m_ClearColour;

	virtual void CreateRenderTargetTexture(const Vector2& size) = 0;
	virtual void InitialiseDearIMGUI(IWindow& window) = 0;

public:
	IRenderer();
	virtual ~IRenderer() = 0;

	const bool IsInitialised() const;
	virtual void Startup(const IWindow& window)  = 0;
	virtual void Shutdown() = 0;

	RenderLayer& GetRenderLayer(const RENDER_LAYER& layer);

	virtual void SetPrimativeDrawColour(const Colour& colour) = 0;
	virtual void SetClearColour(const Colour& colour) = 0;
	virtual void TakeScreenshot(const std::string& name) = 0;

	virtual void EndFrame() = 0;
	virtual void StartFrame() = 0;

	virtual void Render_Point(const Vector2& point) = 0;
	virtual void Render_WorldSpaceLine(const Vector2& pointA, const Vector2& pointB) = 0;
	virtual void Render_ScreenSpaceLine(const Vector2& pointA, const Vector2& pointB) = 0;

	virtual void Render_WorldSpaceRectangle(const WorldRectangle& rectangle,   bool filled = true) = 0;
	virtual void Render_ScreenSpaceRectangle(const ScreenRectangle& rectangle, bool filled = true) = 0;
	virtual void Render_Texture(const Texture& texture, const Transform& transform, const bool& flipped = false) = 0;
	virtual void Render_Texture(const Texture& texture, const Transform& transform, const Vector2* center = nullptr, const WorldRectangle* srcRect = nullptr, const WorldRectangle* dstRect = nullptr, const bool& flipped = false) = 0;
};

