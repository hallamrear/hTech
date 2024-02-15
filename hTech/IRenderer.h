#pragma once
#include "RenderLayer.h"
#include "Colour.h"
#include "Rectangle.h"
#include "Transform.h"

class Texture;
class IWindow;
class RenderTexture;

const size_t c_MaxSpritebatchQuads = 1024;
const size_t c_MaxSpritebatchVertices = c_MaxSpritebatchQuads * 4;
const size_t c_MaxSpritebatchIndices = c_MaxSpritebatchQuads * 6;

class IRenderer
{
private:

protected:
	bool m_IsInitialised;
	RenderLayer m_Layers[(int)RENDER_LAYER::COUNT];
	Colour m_ClearColour;

	virtual void CreateRenderTargetTexture(const Vector2& size) = 0;
	virtual void InitialiseDearIMGUI(IWindow& window) = 0;

	virtual void StartBatch() = 0;
	virtual void EndBatch() = 0;
	virtual void FlushBatch() = 0;

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

