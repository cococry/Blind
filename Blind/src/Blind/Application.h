#pragma once
#include <Blind/Core.h>
#include <Blind/Window.h>
#include <Blind/Events/ApplicationEvent.h>

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
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};
	Application* CreateApplication();
}