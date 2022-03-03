#include "blindpch.h"
#include "RenderCommand.h"

#include <Platform/OpenGL/OpenGLRendererAPI.h>

namespace Blind
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}