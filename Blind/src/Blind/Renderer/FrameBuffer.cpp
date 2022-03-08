#include "blindpch.h"
#include "FrameBuffer.h"

#include <Platform/OpenGL/OpenGLFrameBuffer.h>
#include <Blind/Renderer/Renderer.h>

namespace Blind
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: BLIND_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec);
		}
		return nullptr;
		BLIND_ENGINE_ASSERT(false, "Unknown Rendering API is being used!");
	}

}