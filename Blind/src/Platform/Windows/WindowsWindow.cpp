#include "blindpch.h"
#include "WindowsWindow.h"
#include <Blind/Log.h>

namespace Blind
{
	static bool s_InitializedGLFW = false;

	Window* Window::Create(const WindowProperties& props)
	{
		return new WindowsWindow(props);
	}
	WindowsWindow::WindowsWindow(const WindowProperties& props)
	{
		this->Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		this->Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVsync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.vsync = enabled;
	}

	bool WindowsWindow::IsVsync() const
	{
		return m_Data.vsync;
	}

	void WindowsWindow::Init(const WindowProperties& props)
	{
		m_Data.width = props.width;
		m_Data.height = props.height;
		m_Data.title = props.title;

		BLIND_ENGINE_INFO("Creating GLFW Window {0}: ({1}x{2})", props.title, props.width, props.height);

		if (!s_InitializedGLFW)
		{
			int success = glfwInit();
			BLIND_ENGINE_ASSERT(success, "Failed to initialize GLFW!");
			s_InitializedGLFW = true;
		}

		m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVsync(true);
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}

