#pragma once
#include <Blind/Core.h>
#include <Blind/Window.h>
#include <Blind/Events/ApplicationEvent.h>
#include <Blind/LayerStack.h>

namespace Blind
{
	class BLIND_API Application
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

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
		static Application* s_Instance;
	};
	Application* CreateApplication();
}