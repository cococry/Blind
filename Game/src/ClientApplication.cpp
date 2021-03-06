#include <BlindEngine.h>
#include <Blind/Core/EntryPoint.h>
#include <Platform/OpenGL/OpenGLShader.h>

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Blind::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController((float)APPLICATION_WINDOW.GetWidth() / (float)APPLICATION_WINDOW.GetHeight())
	{
	}

	virtual void OnAttach() override
	{
		m_VertexArray = Blind::VertexArray::Create();

		float vertices[3 * 7] =
		{
			-0.5f,-0.5f, 0.0f,	0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f,-0.5f, 0.0f,	0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f, 0.5f, 0.0f,	0.8f, 0.8f, 0.2f, 1.0f
		};

		Blind::Ref<Blind::VertexBuffer> vertexBuffer;
		vertexBuffer = Blind::VertexBuffer::Create(vertices, sizeof(vertices));

		Blind::BufferLayout layout =
		{
			{ Blind::ShaderDataType::Float3, "a_Position" },
			{ Blind::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };
		Blind::Ref<Blind::IndexBuffer> indexBuffer;
		indexBuffer = Blind::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Blind::VertexArray::Create();


		float squareVertices[4 * 5] =
		{
			-0.5f,-0.5f, 0.0f,		0.0f, 0.0f,
			 0.5f,-0.5f, 0.0f,		1.0f, 0.0f,
			 0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		0.0f, 1.0f
		};

		Blind::Ref<Blind::VertexBuffer> squareVB;
		squareVB = Blind::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout(
			{
				{ Blind::ShaderDataType::Float3, "a_Position" },
				{ Blind::ShaderDataType::Float2, "a_TexCoord" },
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Blind::Ref<Blind::IndexBuffer> squareIB;
		squareIB = Blind::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		m_Shader = Blind::Shader::Create("assets/shaders/Default.glsl");
		m_FlatColorShader = Blind::Shader::Create("assets/shaders/FlatColor.glsl");
		auto& textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Blind::Texture2D::Create("assets/textures/checkerboard.png");
		m_PlayerTexture = Blind::Texture2D::Create("assets/textures/player.png");

		std::dynamic_pointer_cast<Blind::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Blind::OpenGLShader>(textureShader)->UploadInt("u_Texture", 0);
	}

	virtual void OnUpdate(Blind::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Blind::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Blind::RenderCommand::Clear();

		Blind::Renderer::BeginScene(m_CameraController.GetCamera());


		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Blind::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Blind::OpenGLShader>(m_FlatColorShader)->UploadVec3("u_Color", m_SquareColor);																				

		for (int x = 0; x < 20; x++)
		{
			for (int y = 0; y < 20; y++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Blind::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Blind::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_PlayerTexture->Bind();
		Blind::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Blind::Renderer::EndScene();
	}					 

	virtual void OnEvent(Blind::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

	virtual void OnImGuiDraw() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
	virtual void OnDetach() override
	{
	
	}
private:
	Blind::ShaderLibrary m_ShaderLibrary;
	Blind::Ref<Blind::Shader> m_Shader;
	Blind::Ref<Blind::Shader> m_FlatColorShader, m_TextureShader;
	Blind::Ref<Blind::VertexArray> m_VertexArray;

	Blind::Ref<Blind::Texture2D> m_Texture, m_PlayerTexture;

	Blind::Ref<Blind::VertexArray> m_SquareVA;

	Blind::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class SandboxApp : public Blind::Application
{
public:
	SandboxApp()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~SandboxApp()
	{

	}
};

Blind::Application* Blind::CreateApplication()
{
	return new SandboxApp();
}