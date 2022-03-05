#include <blindpch.h>
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <Blind/Core/Core.h>
#include <Blind/Core/Log.h>

namespace Blind
{

	OpenGLContext::OpenGLContext(GLFWwindow* window_handle)
		: m_WindowHandle(window_handle)
	{
		BLIND_ENGINE_ASSERT(window_handle, "No Window Handle specified!");
	}

	void OpenGLContext::Init()
	{
		BL_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int32_t status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		BLIND_ENGINE_ASSERT(status, "Library GLAD: Failed to initialize Glad!");

		BLIND_ENGINE_INFO("OpenGL Renderer: {0} {1}", glGetString(GL_VENDOR), glGetString(GL_RENDERER));

#ifdef BLIND_ENABLE_ASSERTS
		int versionMajor, versionMinor;

		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		BLIND_ENGINE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Blind engine requires at least OpenGL 4.5 to run!");
#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		BL_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}