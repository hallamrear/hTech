#pragma once
#include "Rendering/IRenderer.h"
#include "Rendering/Vertex.h"

typedef unsigned int GLuint;

class OpenGLRenderer :
	public IRenderer
{
private:
	struct BatchData
	{
		Vertex* VertexData = nullptr;
		Vertex* CurrentVertexPointer = nullptr;

		size_t IndexCount = 0;

		unsigned int DrawCount = 0;
		unsigned int QuadCount = 0;
	} m_BatchData;

	GLuint m_IndexBuffer;
	GLuint m_VertexBuffer;
	GLuint m_VertexAttribs;

	bool m_InBatch = false;
	const unsigned int c_QuadIndices[6] = {0, 1, 2, 2, 3, 0};

	void CreateRenderTargetTexture(const Vector2& size);
	void InitialiseDearIMGUI(IWindow& window);

	void StartBatch();
	void EndBatch();
	void FlushBatch();

	SDL_GLContext m_Context;
	bool InitialiseOpenGL(const IWindow& window);
	void ShutdownOpenGL();
	bool SetupSpritebatch();

	bool CreateDefaultShaders();
	void DestroyDefaultShaders();

public:
	OpenGLRenderer();
	~OpenGLRenderer();

	void Startup(const IWindow& window);
	void Shutdown();
	void SetPrimativeDrawColour(const Colour& colour);
	void SetClearColour(const Colour& colour);
	void TakeScreenshot(const std::string& name);
	void SetViewport(const int& x, const int& y, const int& w, const int& h);
	void SetScissorRect(const ScreenRectangle& scissorRect);
	void SetRenderTarget(ITexture* texture);

	void EndFrame();
	void StartFrame();

	ITexture* CreateTexture(const int& width, const int& height) override;
	ITexture* LoadTexture(const std::string& texture_path, const std::string& name) override;
	bool UpdateTexture(ITexture* texture, const int& width, const int& height, const void* data, const size_t& dataSize) override;
	bool DestroyTexture(ITexture* texture) override;

	void CopyTextureToRenderTarget(ITexture* renderTarget, ITexture* texture, ScreenRectangle* srcRect, ScreenRectangle* dstRect);
	void Render_Point(const Vector2& point);
	void Render_WorldSpaceLine(const Vector2& pointA, const Vector2& pointB);
	void Render_ScreenSpaceLine(const Vector2& pointA, const Vector2& pointB);

	void Render_WorldSpaceRectangle(const WorldRectangle& rectangle, bool filled = true);
	void Render_ScreenSpaceRectangle(const ScreenRectangle& rectangle, bool filled = true);
	void Render_Texture(const ITexture& texture, const Transform& transform, const bool& flipped = false);
	void Render_Texture(const ITexture& texture, const Transform& transform, const Vector2* center = nullptr, const WorldRectangle* srcRect = nullptr, const WorldRectangle* dstRect = nullptr, const bool& flipped = false);
};