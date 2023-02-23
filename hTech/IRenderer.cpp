#include "pch.h"
#include "IRenderer.h"

IRenderer::IRenderer()
{
	m_IsInitialised = false;
	m_ClearColour = Colour(0, 0, 0, 255);
}

IRenderer::~IRenderer()
{

}

const bool IRenderer::IsInitialised() const
{
	return m_IsInitialised;
}

RenderLayer& IRenderer::GetRenderLayer(const RENDER_LAYER& layer)
{
	return m_Layers[(int)layer];
}
