#include <blindpch.h>
#include "Application.h"

#include <Blind/Log.h>
#include <Blind/Input.h>

#include <glm/vec3.hpp>
#include <glad/glad.h>

namespace Blind
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		BLIND_ENGINE_ASSERT(!s_Instance, "Blind application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));

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
			glClearColor(0.2f, 0.1f, 0.9f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
 
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
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}

