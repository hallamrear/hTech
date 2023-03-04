#include "pch.h"
#include "OriginalRenderer.h"
#include "Console.h"
#include "Log.h"
#include "Colour.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "Texture.h"
#include "Camera.h"
#include "Transform.h"
#include "OriginalWindow.h"

void OriginalRenderer::CreateRenderTargetTexture(const Vector2& size)
{
	if (m_RenderToTextureTarget != nullptr)
	{
		SDL_DestroyTexture(m_RenderToTextureTarget);
	}

	m_RenderToTextureTarget = SDL_CreateTexture(m_SDLRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)size.X, (int)size.Y);

	if (!m_RenderToTextureTarget)
	{
		SDL_DestroyTexture(m_RenderToTextureTarget);
		m_RenderToTextureTarget = nullptr;
	}
}

OriginalRenderer::OriginalRenderer() : IRenderer()
{
	m_SDLRenderer = nullptr;
}

OriginalRenderer::~OriginalRenderer()
{

}

void OriginalRenderer::Startup(const IWindow& window)
{
	if (m_IsInitialised)
		return;

	//todo : sort out this is disgusting.
	OriginalWindow* sdlWindow = const_cast<OriginalWindow*>(dynamic_cast<const OriginalWindow*>(&window));

	if (sdlWindow == nullptr)
	{
		Log::LogMessage(LogLevel::LOG_WARNING, "Trying to use a non-SDL window to create an SDL Renderer.\n");
		return;
	}

	m_SDLRenderer = SDL_CreateRenderer(sdlWindow->GetAPIWindow(), -1, 0);

	if (m_SDLRenderer)
	{
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Renderer created.");

		Vector2 size;
		sdlWindow->GetWindowSize(size);
		CreateRenderTargetTexture(size);
		if (m_RenderToTextureTarget == nullptr)
		{
			Log::LogMessage(LogLevel::LOG_ERROR, "Failed to render target texture.");
			Log::LogMessage(LogLevel::LOG_ERROR, SDL_GetError());
			Shutdown();
			return;
		}

		Log::LogMessage(LogLevel::LOG_MESSAGE, "Created render target texture.");

		InitialiseDearIMGUI(*sdlWindow);

		SDL_SetRenderDrawBlendMode(m_SDLRenderer, SDL_BLENDMODE_BLEND);

		m_IsInitialised = true;
		return;
	}

	Log::LogMessage(LogLevel::LOG_ERROR, "Renderer failed to create.");
	Shutdown();
	return;
}

void OriginalRenderer::Shutdown()
{
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_SDLRenderer)
	{
		SDL_DestroyRenderer(m_SDLRenderer);
		m_SDLRenderer = nullptr;
	}
}

void OriginalRenderer::TakeScreenshot(const std::string& name)
{
	std::string str;

	if (!m_IsInitialised)
	{
		str = "Tried to take screenshot before renderer is initialised.\n";
		Log::LogMessage(LogLevel::LOG_ERROR, str.c_str());
		return;
	}

	int w, h;
	w = Console::Query("WindowDimensionsW");
	h = Console::Query("WindowDimensionsH");

	SDL_Surface* sshot = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(m_SDLRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);

	if (name == "")
	{
		struct tm _time;
		std::string str = name;
		time_t now = time(nullptr);
		localtime_s(&_time, &now);
		char* buffer = new char[256];
		strftime(buffer, 256, "%d-%m-%Y %H-%M-%S", &_time);
		str = buffer;
	}
	else
	{
		str = name;
	}

	str += ".bmp";
	SDL_SaveBMP(sshot, str.c_str());
	SDL_FreeSurface(sshot);

	str = "Screenshot taken: " + str;

	Log::LogMessage(LogLevel::LOG_MESSAGE, str.c_str());
}

SDL_Renderer* OriginalRenderer::GetAPIRenderer() const
{
	if (m_IsInitialised)
	{
		return m_SDLRenderer;
	}
	else
	{
		Log::LogMessage(LogLevel::LOG_WARNING, "Tried to get the SDL renderer without it being initialised. Returning nullptr.");
		return nullptr;
	}
}

void OriginalRenderer::StartFrame()
{
	SDL_SetRenderTarget(m_SDLRenderer, m_RenderToTextureTarget);
	SDL_SetRenderDrawColor(m_SDLRenderer, m_ClearColour.R, m_ClearColour.G, m_ClearColour.B, m_ClearColour.A);
	SDL_RenderClear(m_SDLRenderer);
}

void OriginalRenderer::EndFrame()
{
	SDL_RenderPresent(m_SDLRenderer);
}


void OriginalRenderer::SetClearColour(const Colour& colour)
{
	m_ClearColour = colour;
}

void OriginalRenderer::SetPrimativeDrawColour(const Colour& colour)
{
	SDL_SetRenderDrawColor(m_SDLRenderer, colour.R, colour.G, colour.B, colour.A);
}

void OriginalRenderer::InitialiseDearIMGUI(IWindow& window)
{
	OriginalWindow* sdlWindow = dynamic_cast<OriginalWindow*>(&window);

	if (sdlWindow == nullptr)
	{
		Log::LogMessage(LogLevel::LOG_WARNING, "Trying to use a non-SDL window to create an SDL Renderer.\n");
		Log::LogMessage(LogLevel::LOG_ERROR, "Failed to initialise DearIMGUI.\n");
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

	// Setup Dear ImGui style
	SettingDearIMGUIColourScheme();


	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(sdlWindow->GetAPIWindow(), m_SDLRenderer);
	ImGui_ImplSDLRenderer_Init(m_SDLRenderer);
	Log::LogMessage(LogLevel::LOG_MESSAGE, "DearIMGUI initialised.");
}

void OriginalRenderer::SettingDearIMGUIColourScheme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 5.3f;
	style.FrameRounding = 2.3f;
	style.ScrollbarRounding = 0;

	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

void OriginalRenderer::Render_Texture(
	const Texture& texture, const Transform& transform,
	const bool& isFlipped)
{
	Render_Texture(texture, transform, nullptr, nullptr, nullptr, isFlipped);
}

void OriginalRenderer::SetRenderTexture(SDL_Texture* texture)
{
	SDL_SetRenderTarget(m_SDLRenderer, texture);
}

SDL_Texture* OriginalRenderer::GetRenderTexture()
{
	return m_RenderToTextureTarget;
}

void OriginalRenderer::Render_Texture(
	const Texture& texture, const Transform& transform,
	const Vector2* center,
	const WorldRectangle* srcRect, const WorldRectangle* dstRect,
	const bool& isFlipped)
{
	Vector2 renderPosition = transform.Position;
	renderPosition.X -= (texture.Width  / 2);
	renderPosition.Y += (texture.Height / 2);
	renderPosition = Camera::WorldToScreen(renderPosition);

	SDL_RendererFlip flip = (SDL_RendererFlip)isFlipped;
	SDL_Point* centerPoint = nullptr;
	SDL_Rect* src = nullptr; 
	SDL_Rect dst;

	if (dstRect)
	{
		Vector2 ssPosition = Camera::WorldToScreen(Vector2(dstRect->X - (dstRect->W / 2), dstRect->Y + (dstRect->H / 2)));
		dst.x = ssPosition.X;
		dst.y = ssPosition.Y;
		dst.w = dstRect->W;
		dst.h = dstRect->H;
	}
	else
	{
		dst.x = renderPosition.X;
		dst.y = renderPosition.Y;
		dst.w = texture.Width;
		dst.h = texture.Height;
	}

	if (center)
	{
		centerPoint = new SDL_Point();
		centerPoint->x = center->X;
		centerPoint->y = center->Y;
	}

	if (srcRect)
	{
		src = new SDL_Rect();
		src->x = srcRect->X;
		src->y = srcRect->Y;
		src->w = srcRect->W;
		src->h = srcRect->H;
	}
	
	SDL_RenderCopyEx(m_SDLRenderer, &texture.GetSDLTexture(), src, &dst, transform.Rotation, centerPoint, flip);

	if (centerPoint)
	{
		delete centerPoint;
		centerPoint = nullptr;
	}

	if (src)
	{
		delete src;
		src = nullptr;
	}
}



void OriginalRenderer::Render_Point(const Vector2& worldSpacePoint)
{
	Vector2 point = Camera::WorldToScreen(worldSpacePoint);
	SDL_RenderDrawPoint(m_SDLRenderer, (int)point.X, (int)point.Y);
}

void OriginalRenderer::Render_ScreenSpaceLine(const Vector2& pointA, const Vector2& pointB)
{
	SDL_RenderDrawLine(m_SDLRenderer,
		(int)pointA.X, (int)pointA.Y,
		(int)pointB.X, (int)pointB.Y);
}

void OriginalRenderer::Render_WorldSpaceLine(const Vector2& pointA, const Vector2& pointB)
{
	Vector2 screenSpacePointA = Camera::WorldToScreen(pointA);
	Vector2 screenSpacePointB = Camera::WorldToScreen(pointB);
	SDL_RenderDrawLine(m_SDLRenderer, 
		(int)screenSpacePointA.X, (int)screenSpacePointA.Y,
		(int)screenSpacePointB.X, (int)screenSpacePointB.Y);
}

void OriginalRenderer::Render_ScreenSpaceRectangle(const ScreenRectangle& rectangle, bool filled)
{
	SDL_Rect rect{};
	Vector2 Position = Vector2();

	rect.w = rectangle.W;
	rect.h = rectangle.H;
	rect.x = rectangle.X - (rect.w / 2);
	rect.y = rectangle.Y - (rect.h / 2);

	if (filled)
	{
		SDL_RenderFillRect(m_SDLRenderer, &rect);
	}
	else
	{
		SDL_RenderDrawRect(m_SDLRenderer, &rect);
	}
}

void OriginalRenderer::Render_WorldSpaceRectangle(const WorldRectangle& rectangle, bool filled)
{
	SDL_Rect rect{};
	Vector2 Position = Vector2(rectangle.X, rectangle.Y);
	Position = Camera::WorldToScreen(Position);
	rect.w = rectangle.W;
	rect.h = rectangle.H;
	rect.x = (int)Position.X - (rectangle.W / 2);
	rect.y = (int)Position.Y - (rectangle.H / 2);

	if (filled)
	{
		SDL_RenderFillRect(m_SDLRenderer, &rect);
	}
	else
	{
		SDL_RenderDrawRect(m_SDLRenderer, &rect);
	}
}