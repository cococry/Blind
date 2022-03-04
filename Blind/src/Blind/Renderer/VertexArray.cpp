#include "blindpch.h"
#include "VertexArray.h"
#include "Renderer.h"

#include <Platform/OpenGL/OpenGLVertexArray.h>

namespace Blind
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: BLIND_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
		}
		return nullptr;
		BLIND_ENGINE_ASSERT(false, "Unknown Rendering API is being used!");
	}
}