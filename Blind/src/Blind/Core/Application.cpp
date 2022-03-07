#include <blindpch.h>
#include "Application.h"

#include <Blind/Core/Log.h>
#include <Blind/Core/Input.h>
#include <Blind/Core/KeyCodes.h>

#include <glm/vec3.hpp>
#include <Blind/Renderer/Renderer.h>
#include <GLFW/glfw3.h>

namespace Blind
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		BL_PROFILE_FUNCTION();

		BLIND_ENGINE_ASSERT(!s_Instance, "Blind application already exists!");
		s_Instance = this;
		m_Window = Window::Create();
		m_Window->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		BL_PROFILE_FUNCTION();
		
		Renderer::Shutdown();
	}

	void Application::Run()
	{
		BL_PROFILE_SCOPE("RunLoop");

		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					BL_PROFILE_SCOPE("Layer Stack Update - Application");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}
				m_ImGuiLayer->Begin();
				{
					BL_PROFILE_SCOPE("Layer Stack OnImGuiRender - Application");
					for (Layer* Layer : m_LayerStack)
						Layer->OnImGuiDraw();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}
	void Application::OnEvent(Event& e)
	{
		BL_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(Application::OnWindowResized));

		if (Input::IsKeyPressed(BL_KEY_ESCAPE))
		{
			m_Running = false;
			BLIND_ENGINE_INFO("Closing Application...");
		}

		for (auto i = m_LayerStack.end(); i != m_LayerStack.begin();)
		{
			(*--i)->OnEvent(e);
			if(e.handled)
				break;
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::PushLayer(Layer* layer)
	{
		BL_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
		BLIND_ENGINE_TRACE("Pushed Layer '{0}' to LayerStack.", layer->GetName());
	}

	void Application::PushOverlay(Layer* overlay)
	{
		BL_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
		BLIND_ENGINE_TRACE("Pushed Overlay '{0}' to LayerStack.", overlay->GetName());
	}

	void Application::PopLayer(Layer* layer)
	{
		BL_PROFILE_FUNCTION();

		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
		BLIND_ENGINE_TRACE("Removed Layer '{0}' from LayerStack.", layer->GetName());
	}

	void Application::PopOverlay(Layer* overlay)
	{

		BL_PROFILE_FUNCTION();

		m_LayerStack.PopOverlay(overlay);
		overlay->OnDetach();
		BLIND_ENGINE_TRACE("Removed Overlay '{0}' from LayerStack.", overlay->GetName());
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		BLIND_ENGINE_INFO("Closing Application...");
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		BL_PROFILE_FUNCTION();

		if (e.GetX() == 0 || e.GetY() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetX(), e.GetY());

		return false;
	}
}

