#include <blindpch.h>
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <Blind/Core.h>
#include <Blind/Log.h>

namespace Blind
{

	OpenGLContext::OpenGLContext(GLFWwindow* window_handle)
		: m_WindowHandle(window_handle)
	{
		BLIND_ENGINE_ASSERT(window_handle, "No Window Handle specified!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int32_t status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		BLIND_ENGINE_ASSERT(status, "Library GLAD: Failed to initialize Glad!");

		BLIND_ENGINE_INFO("OpenGL Renderer: {0} {1}", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}