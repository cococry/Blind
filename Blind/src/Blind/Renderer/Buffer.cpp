#include "blindpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include <Platform/OpenGL/OpenGLBuffer.h>
#include <Blind/Log.h>
#include <Blind/Core.h>


namespace Blind
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: BLIND_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}
		return nullptr;
		BLIND_ENGINE_ASSERT(false, "Unknown Rendering API is being used!"); 
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: BLIND_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, size);
		}
		return nullptr;
		BLIND_ENGINE_ASSERT(false, "Unknown Rendering API is being used!");
	}


}