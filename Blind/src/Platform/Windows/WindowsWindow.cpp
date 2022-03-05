#include "blindpch.h"
#include "WindowsWindow.h"
#include <Blind/Core/Log.h>

#include <Blind/Events/ApplicationEvent.h>
#include <Blind/Events/KeyEvent.h>
#include <Blind/Events/MouseEvent.h>
#include <Platform/OpenGL/OpenGLContext.h>
#include <Blind/Renderer/Renderer.h>

#include <glad/glad.h>
namespace Blind
{
	static bool s_InitializedGLFW = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		BLIND_ENGINE_ERROR("GLFW Error: ({0}): {1}", error, description);
	}		

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
		m_Context->SwapBuffers();
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

		BLIND_ENGINE_INFO("Creating GLFW Window {0}: ({1}x{2}).", props.title, props.width, props.height);

		if (!s_InitializedGLFW)
		{
			int success = glfwInit();
			BLIND_ENGINE_ASSERT(success, "Failed to initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_InitializedGLFW = true;
		}

		m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.title.c_str(), nullptr, nullptr);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVsync(true);
		BLIND_ENGINE_INFO("Successfully created Window: {0} ({1}x{2}) with Rendering API {3}.", props.title, props.width, props.height, Renderer::RendererAPIToString()); 

		// GLFW Callbacks 
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
			});
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action) {
			case GLFW_PRESS: {
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}

			case GLFW_RELEASE: {
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT: {
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			}
			});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action) {
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
			});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});
		
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}

