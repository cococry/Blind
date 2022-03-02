#include "blindpch.h"
#include "Renderer.h"

namespace Blind
{
	RendererAPI Renderer::s_RendererAPI = RendererAPI::OpenGL;

	std::string Renderer::RendererAPIToString()
	{
		switch (s_RendererAPI)
		{
		case RendererAPI::OpenGL:
			return "OpenGL";
		case RendererAPI::None:
			return "None";
		}
		return "";
	}


}