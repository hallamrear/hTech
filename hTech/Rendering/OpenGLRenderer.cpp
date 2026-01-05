#include "pch.h"
#include "Rendering/OpenGLRenderer.h"
#include "System/Console.h"
#include "Rendering/OriginalWindow.h"
#include "Rendering/GlShader.h"
#include <External/OPENGL.h>
#include <External/SDL.h>
#include <External/IMGUI.h>
#include "Rendering/GLTexture.h"

const char* glsl_version = "#version 460 core";

const char* cDefaultVertexShader = " \
#version 460 core\n \
layout(location = 0) in vec3  iPosition;\n\
layout(location = 1) in vec3  iColour;\n\
layout(location = 2) in vec2  iUV;\n\
layout(location = 3) in float iTextureID;\n\
out vec3 oColour;\n\
out vec2 oUV;\n\
out float oID;\n\
uniform mat4 uViewProjection;\n\
uniform mat4 uTransform;\n\
\n\
void main()\n\
{\n\
	oUV = iUV;\n\
	oID = iTextureID;\n\
	oColour = iColour;\n\
	gl_Position = uViewProjection * uTransform * vec4(iPosition, 1.0f);\n\
}\n\
";

const char* cDefaultPixelShader =
"\
#version 460 core\n\
in vec3 oColour;\n\
in vec2 oUV;\n\
in float oID;\n\
layout(location = 0) out vec4 pColour;\n\
void main()\n\
{\n\
	pColour = vec4(oColour, 1.0f);\n\
	pColour = vec4(oUV, 0.0f, 1.0f);\n\
};\n\
";

OpenGLRenderer::OpenGLRenderer()
{
	m_InBatch = false;
	m_Context = nullptr;
	m_VertexAttribs = GL_INVALID_VALUE;
	m_VertexBuffer = GL_INVALID_VALUE;
	m_IndexBuffer = GL_INVALID_VALUE; 
}

OpenGLRenderer::~OpenGLRenderer()
{
	Shutdown();
}

bool OpenGLRenderer::InitialiseOpenGL(const IWindow& window)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	//todo : refactor IWindow and window creation.
	OriginalWindow* oWindow = dynamic_cast<OriginalWindow*>(const_cast<IWindow*>(&window));
	m_Context = SDL_GL_CreateContext(oWindow->GetAPIWindow());

	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::string err = std::string((char*)glewGetErrorString(result));
		Console::LogMessage(LogLevel::LOG_ERROR, err);
		return false;
	}

	Vector2 windowSize;
	window.GetWindowSize(windowSize);

	//Setup viewport.
	glViewport(0, 0, windowSize.X, windowSize.Y);
	
	bool createdShaders = CreateDefaultShaders();
	if (!createdShaders)
	{
		Console::LogMessage(LogLevel::LOG_ERROR, "Failed to create default shaders.");
		return false;
	}

	InitialiseDearIMGUI(*oWindow);

	return true;
}

void OpenGLRenderer::ShutdownOpenGL()
{
	DestroyDefaultShaders();
	SDL_GL_DeleteContext(m_Context);
}

bool OpenGLRenderer::SetupSpritebatch()
{
	//Create the Vertex data array.
	m_BatchData.VertexData = new Vertex[c_MaxSpritebatchVertices];
	m_BatchData.CurrentVertexPointer = m_BatchData.VertexData;

	//Create an empty dynamic GPU vertex buffer.
	//
	glGenBuffers(1, &m_VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * c_MaxSpritebatchVertices, nullptr, GL_DYNAMIC_DRAW);

	//Populate index array
	int indexOffset = 0;
	unsigned int* indices = new unsigned int[c_MaxSpritebatchIndices];
	for (size_t i = 0; i < c_MaxSpritebatchIndices; i += 6)
	{
		//Triangle One
		indices[i + 0] = c_QuadIndices[0] + indexOffset;
		indices[i + 1] = c_QuadIndices[1] + indexOffset;
		indices[i + 2] = c_QuadIndices[2] + indexOffset;

		//Triangle Two
		indices[i + 3] = c_QuadIndices[3] + indexOffset;
		indices[i + 4] = c_QuadIndices[4] + indexOffset;
		indices[i + 5] = c_QuadIndices[5] + indexOffset;
	}

	//Create index buffer using this data. 
	glGenBuffers(1, &m_IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, c_MaxSpritebatchIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//Cleanup array
	delete[] indices;
	indices = nullptr;

	glGenVertexArrays(1, &m_VertexAttribs);
	glBindVertexArray(m_VertexAttribs);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

	/* Position */ 
	glEnableVertexArrayAttrib(m_VertexAttribs, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

	/* Colours */ 
	glEnableVertexArrayAttrib(m_VertexAttribs, 1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Colour));

	/* TexCoords */
	glEnableVertexArrayAttrib(m_VertexAttribs, 2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	/* TextureID */ 
	glEnableVertexArrayAttrib(m_VertexAttribs, 3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureID));


	return true;
}

bool OpenGLRenderer::CreateDefaultShaders()
{
	m_Shader = new GLShader();
	bool success = m_Shader->Create(cDefaultPixelShader, cDefaultVertexShader);

	if (success == false)
	{
		DestroyDefaultShaders();
	}

	return success;
}

void OpenGLRenderer::DestroyDefaultShaders()
{
	if (m_Shader)
	{
		m_Shader->Destroy();
		delete m_Shader;
		m_Shader = nullptr;
	}
}

void OpenGLRenderer::Startup(const IWindow& window)
{
	if (InitialiseOpenGL(window))
	{
		SetupSpritebatch();
		m_IsInitialised = true;
	}
}

void OpenGLRenderer::Shutdown()
{
	if (m_InBatch)
	{
		EndBatch();
	}

	ShutdownOpenGL();

	//Cleanup m_BatchData
	m_BatchData.DrawCount = 0;
	m_BatchData.IndexCount = 0;
	m_BatchData.QuadCount = 0;
	m_BatchData.CurrentVertexPointer = nullptr;
	
	delete[] m_BatchData.VertexData;
	m_BatchData.VertexData = nullptr;

	m_IsInitialised = false;
}

void OpenGLRenderer::StartBatch()
{
	//Set current m_BatchData vertex array pointer to the front of the array.
	m_BatchData.CurrentVertexPointer = m_BatchData.VertexData;
	m_InBatch = true;
}

void OpenGLRenderer::EndBatch()
{
	//todo : double check this is accurate.
	//Get the size of the current batch vertex data by taking the pointer position away from the start.
	size_t batchDataSize = m_BatchData.CurrentVertexPointer - m_BatchData.VertexData;	

	//Map m_BatchData.Vertices to VertexBuffer using dynamic map function
	//e.g. MapMemory/UnmapMemory or glBufferSubData
	size_t offset = 0;
	glBufferSubData(m_VertexBuffer, offset, batchDataSize, m_BatchData.VertexData);

	m_InBatch = false;
}

void OpenGLRenderer::FlushBatch()
{
	if (m_InBatch)
	{
		EndBatch();
	}

	//Draw existing batch data to the screen
	//Bind texture slots using texture ids.
	//glBindTexture(GL_TEXTURE_2D, texture);
	//size_t textureCount = m_ExpectedTextures.size();
	//for (size_t i = 0; i < m_ExpectedTextures; i++)
	//{
	//
	//}

	if (m_BatchData.IndexCount > 0)
	{
		//Draw batch data using index count.
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBindVertexArray(m_VertexAttribs);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		glDrawElements(GL_TRIANGLES, m_BatchData.IndexCount, GL_UNSIGNED_INT, 0);
		m_BatchData.DrawCount++;
	}
}

void OpenGLRenderer::CreateRenderTargetTexture(const Vector2& size)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void OpenGLRenderer::InitialiseDearIMGUI(IWindow& window)
{
	OriginalWindow* sdlWindow = dynamic_cast<OriginalWindow*>(&window);

	if (sdlWindow == nullptr)
	{
		Console::LogMessage(LogLevel::LOG_WARNING, "Trying to use a non-SDL window to create an SDL Renderer.\n");
		Console::LogMessage(LogLevel::LOG_ERROR, "Failed to initialise DearIMGUI.\n");
		return;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingWithShift = true;
	io.ConfigFlags |= ImGuiDockNodeFlags_PassthruCentralNode;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	SDL_GL_MakeCurrent(sdlWindow->GetAPIWindow(), m_Context);
	ImGui_ImplSDL2_InitForOpenGL(sdlWindow->GetAPIWindow(), m_Context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	Console::LogMessage(LogLevel::LOG_MESSAGE, "DearIMGUI initialised.");
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void OpenGLRenderer::SetPrimativeDrawColour(const Colour& colour)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void OpenGLRenderer::SetClearColour(const Colour& colour)
{
	glClearColor(colour.R / 255.0f, colour.G / 255.0f, colour.B / 255.0f, colour.A / 255.0f);
	return;
}

void OpenGLRenderer::TakeScreenshot(const std::string& name)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void OpenGLRenderer::SetViewport(const int& x, const int& y, const int& w, const int& h)
{
	glViewport(x, y, w, h);
}

void OpenGLRenderer::SetScissorRect(const ScreenRectangle& scissorRect)
{
	glScissor(scissorRect.X, scissorRect.Y, scissorRect.W, scissorRect.H);
}

void OpenGLRenderer::SetRenderTarget(ITexture* texture)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void OpenGLRenderer::StartFrame()
{
	ImGui_ImplSDL2_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");

	glClear(GL_COLOR_BUFFER_BIT);


	//Setup uniform matrices.
	//Set transform matrix.
	//Set ViewProj matrix.

	m_Shader->Bind();

	return;
}

ITexture* OpenGLRenderer::CreateTexture(const int& width, const int& height)
{
	if (width <= 0 || height <= 0)
		return nullptr;

	GLTexture* texture = new GLTexture();

	bool success = texture->Create(width, height);

	if (!success && texture->Exists())
	{
		delete texture;
		texture = nullptr;
	}

	return texture;
}

ITexture* OpenGLRenderer::LoadTexture(const std::string& texture_path, const std::string& name)
{
	if (texture_path == "" || name == "")
		return nullptr;

	GLTexture* texture = new GLTexture();

	bool success = texture->Load(texture_path, name);

	if (!success && texture->Exists())
	{
		delete texture;
		texture = nullptr;
	}

	return texture;
}

bool OpenGLRenderer::UpdateTexture(ITexture* texture, const int& width, const int& height, const void* data, const size_t& dataSize)
{
	if (width <= 0 || height <= 0)
		return false;

	if (texture == nullptr)
		return false;

	GLTexture* glTexture = dynamic_cast<GLTexture*>(texture);

	if (glTexture == nullptr)
		return false;

	glActiveTexture(glTexture->GetID());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	return false;
}

bool OpenGLRenderer::DestroyTexture(ITexture* texture)
{
	if (texture == nullptr)
		return false;

	GLTexture* glTexture = dynamic_cast<GLTexture*>(texture);

	if (glTexture)
	{
		return glTexture->Destroy();
	}

	return false;
}

void OpenGLRenderer::EndFrame()
{
	ImGui::Begin("Renderer data");

	ImGui::Text("Draw calls: %i", m_BatchData.DrawCount);
	ImGui::Text("Quads rendered: % i", m_BatchData.QuadCount);

	ImGui::End();

	m_BatchData.DrawCount = 0;
	m_BatchData.QuadCount = 0;



	//Swap frame buffers
	SDL_GL_SwapWindow(m_Window);

	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void OpenGLRenderer::CopyTextureToRenderTarget(ITexture* renderTarget, ITexture* texture, ScreenRectangle* srcRect, ScreenRectangle* dstRect)
{
	if (renderTarget == nullptr)
	{
		//Use current render target.
	}

	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void OpenGLRenderer::Render_Point(const Vector2& point)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void OpenGLRenderer::Render_WorldSpaceLine(const Vector2& pointA, const Vector2& pointB)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void OpenGLRenderer::Render_ScreenSpaceLine(const Vector2& pointA, const Vector2& pointB)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void OpenGLRenderer::Render_WorldSpaceRectangle(const WorldRectangle& rectangle, bool filled)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void OpenGLRenderer::Render_ScreenSpaceRectangle(const ScreenRectangle& rectangle, bool filled)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void OpenGLRenderer::Render_Texture(const ITexture& texture, const Transform& transform, const bool& flipped)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void OpenGLRenderer::Render_Texture(const ITexture& texture, const Transform& transform, const Vector2* center, const WorldRectangle* srcRect, const WorldRectangle* dstRect, const bool& flipped)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}