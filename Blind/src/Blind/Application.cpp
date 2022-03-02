#include <blindpch.h>
#include "Application.h"

#include <Blind/Log.h>
#include <Blind/Input.h>

#include <glm/vec3.hpp>
#include <glad/glad.h>

namespace Blind
{
	Application* Application::s_Instance = nullptr;

	
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Blind::ShaderDataType::Float:		return GL_FLOAT;
		case Blind::ShaderDataType::Float2:		return GL_FLOAT;
		case Blind::ShaderDataType::Float3:		return GL_FLOAT;
		case Blind::ShaderDataType::Float4:		return GL_FLOAT;
		case Blind::ShaderDataType::Mat3:		return GL_FLOAT;
		case Blind::ShaderDataType::Mat4:		return GL_FLOAT;
		case Blind::ShaderDataType::Int:		return GL_INT;
		case Blind::ShaderDataType::Int2:		return GL_INT;
		case Blind::ShaderDataType::Int3:		return GL_INT;
		case Blind::ShaderDataType::Int4:		return GL_INT;
		case Blind::ShaderDataType::Bool:		return GL_BOOL;
		}
		BLIND_ENGINE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}


	Application::Application()
	{
		BLIND_ENGINE_ASSERT(!s_Instance, "Blind application already exists!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 7] =
		{
			-0.5f,-0.5f, 0.0f,	0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f,-0.5f, 0.0f,	0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f, 0.5f, 0.0f,	0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = 
		{
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		
		m_VertexBuffer->SetLayout(layout);

		uint32_t index = 0;
		for (const auto& element : m_VertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type), 
				element.Normalized ? GL_TRUE : GL_FALSE, 
				layout.GetStride(), 
				(const void*)(element.Offset));
			index++;
		}

	

		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSource = R"(
		#version 330 core
		
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;
		
		out vec3 v_Position;
		out vec4 v_Color;

		void main()
		{
			v_Position = a_Position;
			v_Color = a_Color;
			gl_Position = vec4(a_Position - 0.1, 1.0);
		}
		)";
		std::string fragmentSource = R"(
		#version 330 core
		
		layout(location = 0) out vec4 color;

		in vec3 v_Position;
		in vec4 v_Color;
		
		void main()
		{
			color = vec4(v_Position * 0.5 + 0.5, 1.0);
			color = v_Color;
		}
		)";

		m_Shader.reset(new Shader(vertexSource, fragmentSource));
		m_Shader->Bind();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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

