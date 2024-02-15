#include "pch.h"
#include "SpritebatchRenderer.h"
#include "Console.h"

SpritebatchRenderer::SpritebatchRenderer()
{
	m_InBatch = false;
}

SpritebatchRenderer::~SpritebatchRenderer()
{

}

void SpritebatchRenderer::Startup(const IWindow& window)
{
	//Create the Vertex data array.
	m_BatchData.VertexData = new Vertex[c_MaxSpritebatchVertices];
	m_BatchData.CurrentVertexPointer = m_BatchData.VertexData;

	//Create an empty dynamic GPU vertex buffer.
	//todo : implement
	
	//Populate index array
	int indexOffset = 0;
	unsigned int*  indices = new unsigned int[c_MaxSpritebatchIndices];
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
	//todo : implement
	
	//Cleanup array
	delete[] indices;
	indices = nullptr;

	m_IsInitialised = true;
}

void SpritebatchRenderer::Shutdown()
{
	if (m_InBatch)
	{
		EndBatch();
	}

	//Cleanup m_BatchData
	m_BatchData.DrawCount = 0;
	m_BatchData.IndexCount = 0;
	m_BatchData.QuadCount = 0;
	m_BatchData.CurrentVertexPointer = nullptr;
	
	delete[] m_BatchData.VertexData;
	m_BatchData.VertexData = nullptr;

	m_IsInitialised = false;
}

void SpritebatchRenderer::StartBatch()
{
	//Set current m_BatchData vertex array pointer to the front of the array.
	m_BatchData.CurrentVertexPointer = m_BatchData.VertexData;
	m_InBatch = true;
}

void SpritebatchRenderer::EndBatch()
{
	//todo : double check this is accurate.
	//Get the size of the current batch vertex data by taking the pointer position away from the start.
	size_t batchDataSize = m_BatchData.CurrentVertexPointer - m_BatchData.VertexData;	

	//Map m_BatchData.Vertices to VertexBuffer using dynamic map function
	//e.g. MapMemory/UnmapMemory or glBufferSubData
	m_InBatch = false;
}

void SpritebatchRenderer::FlushBatch()
{
	if (m_InBatch)
	{
		EndBatch();
	}

	//Draw existing batch data to the screen and reset.
	//Bind texture slots using texture ids.

	//Draw batch data using m_BatchData.Vertices and index count.

	m_BatchData.DrawCount++;
}

void SpritebatchRenderer::CreateRenderTargetTexture(const Vector2& size)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void SpritebatchRenderer::InitialiseDearIMGUI(IWindow& window)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void SpritebatchRenderer::SetPrimativeDrawColour(const Colour& colour)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void SpritebatchRenderer::SetClearColour(const Colour& colour)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void SpritebatchRenderer::TakeScreenshot(const std::string& name)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void SpritebatchRenderer::EndFrame()
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void SpritebatchRenderer::StartFrame()
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void SpritebatchRenderer::Render_Point(const Vector2& point)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void SpritebatchRenderer::Render_WorldSpaceLine(const Vector2& pointA, const Vector2& pointB)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void SpritebatchRenderer::Render_ScreenSpaceLine(const Vector2& pointA, const Vector2& pointB)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void SpritebatchRenderer::Render_WorldSpaceRectangle(const WorldRectangle& rectangle, bool filled)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void SpritebatchRenderer::Render_ScreenSpaceRectangle(const ScreenRectangle& rectangle, bool filled)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void SpritebatchRenderer::Render_Texture(const Texture& texture, const Transform& transform, const bool& flipped)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}

void SpritebatchRenderer::Render_Texture(const Texture& texture, const Transform& transform, const Vector2* center, const WorldRectangle* srcRect, const WorldRectangle* dstRect, const bool& flipped)
{
	Console::LogMessage(LogLevel::LOG_ERROR, "Function not implemented.");
	return;
}
