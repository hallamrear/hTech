#pragma once
#include "RenderLayer.h"
#include "Colour.h"
#include "Rectangle.h"
#include "Transform.h"

enum class RENDER_LAYER
{
	DEFAULT = 0,
	BACKGROUND = 1,
	FOREGROUND = 2,

	LAYER_TO_BE_REMOVED_WHEN_I_HOOK_UP_LAYERS_TO_COMPONENT = 9999
};

class Texture;
class IWindow;
class RenderTexture;

class IRenderer
{

private:

protected:
	bool m_IsInitialised;
	RenderLayer m_Layers[3];
	Colour m_ClearColour;

	virtual void CreateRenderTargetTexture(const Vector2& size) = 0;
	virtual void InitialiseDearIMGUI(IWindow& window) = 0;

public:
	IRenderer();
	virtual ~IRenderer() = 0;

	const bool IsInitialised() const;
	virtual void Startup(const IWindow& window)  = 0;
	virtual void Shutdown() = 0;

	virtual void SetPrimativeDrawColour(const Colour& colour) = 0;
	virtual void SetClearColour(const Colour& colour) = 0;
	virtual void TakeScreenshot(const std::string& name) = 0;

	virtual void EndFrame() = 0;
	virtual void StartFrame() = 0;

	virtual void Render_Point(const Vector2& point) = 0;
	virtual void Render_Line(const Vector2& pointA, const Vector2& pointB) = 0;

	virtual void Render_WorldSpaceRectangle(const WorldRectangle& rectangle, const RENDER_LAYER& layer = RENDER_LAYER::DEFAULT, bool filled = true, bool drawCentered = false) = 0;
	virtual void Render_ScreenSpaceRectangle(const ScreenRectangle& rectangle, const RENDER_LAYER& layer = RENDER_LAYER::DEFAULT, bool filled = true, bool drawCentered = false) = 0;

	virtual void Render_Texture(const Texture& texture, const Transform& transform, const RENDER_LAYER& layer = RENDER_LAYER::DEFAULT, const bool& flipped = false) = 0;
	virtual void Render_Texture(const Texture& texture, const Transform& transform, const RENDER_LAYER& layer = RENDER_LAYER::DEFAULT, const Vector2* center = nullptr, const WorldRectangle* srcRect = nullptr, const WorldRectangle* dstRect = nullptr, const bool& flipped = false) = 0;
};

