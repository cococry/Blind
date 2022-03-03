#include <blindpch.h>
#include "Application.h"

#include <Blind/Log.h>
#include <Blind/Input.h>

#include <glm/vec3.hpp>
#include <Blind/Renderer/Renderer.h>

namespace Blind
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		BLIND_ENGINE_ASSERT(!s_Instance, "Blind application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] =
		{
			-0.5f,-0.5f, 0.0f,	0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f,-0.5f, 0.0f,	0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f, 0.5f, 0.0f,	0.8f, 0.8f, 0.2f, 1.0f
		};

		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = 
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };
		Ref<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(VertexArray::Create());


		float squareVertices[3 * 4] =
		{
			-0.5f,-0.5f, 0.0f,	
			 0.5f,-0.5f, 0.0f,	
			 0.5f, 0.5f, 0.0f,	
			-0.5f, 0.5f, 0.0f,	
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout(
		{
			{ ShaderDataType::Float3, "a_Position" },
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSource = R"(
		#version 330 core
		
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;

		uniform mat4 u_ViewProjection;		
			
		out vec3 v_Position;
		out vec4 v_Color;

		void main()
		{
			v_Position = a_Position;
			v_Color = a_Color;
			gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
		}
		)";
		std::string fragmentSource = R"(
		#version 330 core
		
		layout(location = 0) out vec4 color;

		in vec3 v_Position;
		in vec4 v_Color;
		
		void main()
		{
			color = v_Color;
		}
		)";

		m_Shader.reset(new Shader(vertexSource, fragmentSource));


		std::string vertexSourceBlue = R"(
		#version 330 core
		
		layout(location = 0) in vec3 a_Position;
		
		uniform mat4 u_ViewProjection;				

		out vec3 v_Position;
		
		void main()
		{
			v_Position = a_Position;
			gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
		}
		)";
		std::string fragmentSourceBlue = R"(
		#version 330 core
		
		layout(location = 0) out vec4 color;

		in vec3 v_Position;

		void main()
		{
			color = vec4(0.2, 0.3, 0.8, 1.0);
		}
		)";

		m_BlueShader.reset(new Shader(vertexSourceBlue, fragmentSourceBlue));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
			m_Camera.SetRotation(45.0f);

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_BlueShader, m_SquareVA);
			Renderer::Submit(m_Shader, m_VertexArray);

			Renderer::EndScene();

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
}

