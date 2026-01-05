#pragma once
#include <Rendering/RenderLayer.h>
#include <Rendering/Colour.h>
#include <Rendering/Rectangle.h>
#include <DataTypes/Transform.h>
#include <External/IMGUI.h>
#include <Rendering/IShader.h>

class ITexture;
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

	IShader* m_Shader;

public:
	IRenderer();
	virtual ~IRenderer() = 0;

	const bool IsInitialised() const;
	virtual void Startup(const IWindow& window)  = 0;
	virtual void Shutdown() = 0;

	RenderLayer& GetRenderLayer(const RENDER_LAYER& layer);

	virtual void SetViewport(const int& x, const int& y, const int& w, const int& h) = 0;
	virtual void SetScissorRect(const ScreenRectangle& scissorRect) = 0;

	virtual void SetPrimativeDrawColour(const Colour& colour) = 0;
	virtual void SetClearColour(const Colour& colour) = 0;
	virtual void TakeScreenshot(const std::string& name) = 0;

	virtual void EndFrame() = 0;
	virtual void StartFrame() = 0;

	virtual void StartBatch() = 0;
	virtual void EndBatch() = 0;
	virtual void FlushBatch() = 0;

	virtual ITexture* CreateTexture(const int& width, const int& height) = 0;
	virtual ITexture* LoadTexture(const std::string& texture_path, const std::string& name) = 0;
	virtual bool UpdateTexture(ITexture* texture, const int& width, const int& height, const void* data, const size_t& dataSize) = 0;
	virtual bool DestroyTexture(ITexture* texture) = 0;

	virtual void SetRenderTarget(ITexture* texture) = 0;
	virtual void CopyTextureToRenderTarget(ITexture* renderTarget, ITexture* texture, ScreenRectangle* srcRect, ScreenRectangle* dstRect) = 0;

	virtual void Render_Point(const Vector2& point) = 0;
	virtual void Render_WorldSpaceLine(const Vector2& pointA, const Vector2& pointB) = 0;
	virtual void Render_ScreenSpaceLine(const Vector2& pointA, const Vector2& pointB) = 0;

	virtual void Render_WorldSpaceRectangle(const WorldRectangle& rectangle,   bool filled = true) = 0;
	virtual void Render_ScreenSpaceRectangle(const ScreenRectangle& rectangle, bool filled = true) = 0;
	virtual void Render_Texture(const ITexture& texture, const Transform& transform, const bool& flipped = false) = 0;
	virtual void Render_Texture(const ITexture& texture, const Transform& transform, const Vector2* center = nullptr, const WorldRectangle* srcRect = nullptr, const WorldRectangle* dstRect = nullptr, const bool& flipped = false) = 0;
};

