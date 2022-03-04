#include <BlindEngine.h>
#include <Platform/OpenGL/OpenGLShader.h>

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Blind::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f, 0.0f, 0.0f)
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

		std::string vertexSource = R"(
		#version 330 core
		
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;

		uniform mat4 u_ViewProjection;		
		uniform mat4 u_Transform;		
			
		out vec3 v_Position;
		out vec4 v_Color;

		void main()
		{
			v_Position = a_Position;
			v_Color = a_Color;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

		m_Shader = Blind::Shader::Create("VertexPosColor", vertexSource, fragmentSource);


		std::string flatColorShaderVertexSource = R"(
		#version 330 core
		
		layout(location = 0) in vec3 a_Position;
		
		uniform mat4 u_ViewProjection;				
		uniform mat4 u_Transform;		

		out vec3 v_Position;
		
		void main()
		{
			v_Position = a_Position;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
		}
		)";
		std::string flatColorShaderFragmentSource = R"(
		#version 330 core
		
		layout(location = 0) out vec4 color;

		in vec3 v_Position;

		uniform vec3 u_Color;

		void main()
		{
			color = vec4(u_Color, 1.0);
		}
		)";

		m_FlatColorShader = Blind::Shader::Create("FlatColor", flatColorShaderVertexSource, flatColorShaderFragmentSource);

		auto& textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Blind::Texture2D::Create("assets/textures/checkerboard.png");
		m_PlayerTexture = Blind::Texture2D::Create("assets/textures/player.png");

		std::dynamic_pointer_cast<Blind::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Blind::OpenGLShader>(textureShader)->UploadInt("u_Texture", 0);
	}

	virtual void OnUpdate(Blind::Timestep ts) override
	{
		if(Blind::Input::IsKeyPressed(BL_KEY_A))
			m_CameraPosition.x -= m_MoveCameraSpeed * ts;
		else if (Blind::Input::IsKeyPressed(BL_KEY_D))
			m_CameraPosition.x += m_MoveCameraSpeed * ts;

		if (Blind::Input::IsKeyPressed(BL_KEY_W))
			m_CameraPosition.y += m_MoveCameraSpeed * ts;

		else if (Blind::Input::IsKeyPressed(BL_KEY_S))
			m_CameraPosition.y -= m_MoveCameraSpeed * ts;

		if (Blind::Input::IsKeyPressed(BL_KEY_LEFT))
			m_CameraRotation += m_CameraRotationSpeed * ts;

		else if (Blind::Input::IsKeyPressed(BL_KEY_RIGHT))
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		

		Blind::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Blind::RenderCommand::Clear();
		
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Blind::Renderer::BeginScene(m_Camera);


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

		// Triangle
		//Blind::Renderer::Submit(m_Shader, m_VertexArray);

		Blind::Renderer::EndScene();
	}					 

	virtual void OnEvent(Blind::Event& e) override
	{
		 
	}

	virtual void OnImGuiDraw() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
private:
	Blind::ShaderLibrary m_ShaderLibrary;
	Blind::Ref<Blind::Shader> m_Shader;
	Blind::Ref<Blind::Shader> m_FlatColorShader, m_TextureShader;
	Blind::Ref<Blind::VertexArray> m_VertexArray;

	Blind::Ref<Blind::Texture2D> m_Texture, m_PlayerTexture;

	Blind::Ref<Blind::VertexArray> m_SquareVA;

	Blind::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;
	float m_MoveCameraSpeed = 5.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class SandboxApp : public Blind::Application
{
public:
	SandboxApp()
	{
		PushLayer(new ExampleLayer());
	}
	~SandboxApp()
	{

	}
};

Blind::Application* Blind::CreateApplication()
{
	return new SandboxApp();
}
