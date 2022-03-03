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

		bool m_Running = true;

		Ref<Shader> m_Shader;
		Ref<Shader> m_BlueShader;
		Ref<VertexArray> m_VertexArray;

		Ref<VertexArray> m_SquareVA;

		OrthographicCamera m_Camera;

		Scope<Window> m_Window;

		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};
	Application* CreateApplication();
}