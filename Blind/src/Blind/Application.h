#pragma once
#include <Blind/Core.h>
#include <Blind/Window.h>
#include <Blind/Events/ApplicationEvent.h>
#include <Blind/LayerStack.h>
#include <Blind/ImGui/ImGuiLayer.h>
#include <Blind/Renderer/Shader.h>
#include <Blind/Renderer/Buffer.h>
#include <Blind/Renderer/VertexArray.h>
#include <Blind/Renderer/OrthographicCamera.h>
#include <Blind/Core/Timestep.h>

namespace Blind
{
	class Application
	{
	/* \Blind Application Class
		This class is used to create a Application with Blind. It is implemented client side for now.
		It is DLL exported by Blind into the Client. The class will be created only by client side. */
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClosed(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		bool m_Running = true;
		bool m_Minimized = false;

		Scope<Window> m_Window;

		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f; // Time that it took to render last frame

		static Application* s_Instance;
	};
	Application* CreateApplication();
}