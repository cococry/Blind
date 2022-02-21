#pragma once

#include <Blind/Renderer/GraphicsContext.h>

struct GLFWwindow;

namespace Blind
{
	class OpenGLContext : public GraphicsContext
	{
	public:	
		OpenGLContext(GLFWwindow* window_handle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}