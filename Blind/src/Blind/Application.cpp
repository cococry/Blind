#include <blindpch.h>
#include "Application.h"

#include <Blind/Log.h>
#include <Blind/Input.h>
#include <Blind/KeyCodes.h>

#include <glm/vec3.hpp>
#include <Blind/Renderer/Renderer.h>
#include <GLFW/glfw3.h>

namespace Blind
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		BLIND_ENGINE_ASSERT(!s_Instance, "Blind application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}
			m_ImGuiLayer->Begin();

			for (Layer* Layer : m_LayerStack)
				Layer->OnImGuiDraw();

			m_ImGuiLayer->End();
			m_Window->OnUpdate();
		}
	}
	void Application::OnEvent(Event& e)
	{
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

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
		BLIND_ENGINE_TRACE("Pushed Layer '{0}' to LayerStack.", layer->GetName());
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
		BLIND_ENGINE_TRACE("Pushed Overlay '{0}' to LayerStack.", overlay->GetName());
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		BLIND_ENGINE_INFO("Closing Application...");
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
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

