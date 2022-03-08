
#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController((float)APPLICATION_WINDOW.GetWidth() / (float)APPLICATION_WINDOW.GetHeight())
{
}

void Sandbox2D::OnAttach()
{
	BL_PROFILE_FUNCTION();

	m_CheckerboardTexture = Blind::Texture2D::Create("assets/textures/checkerboard.png");
	m_SpriteSheet = Blind::Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");

	m_TextureStairs = Blind::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 11 }, { 128, 128 });
	m_TextureTree = Blind::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 4 }, { 128, 128 }, {1, 2});

	m_CameraController.SetZoomLevel(5.0f); 

	Blind::FrameBufferSpecification frameBufferSpecification;
	frameBufferSpecification.Width = APPLICATION_WINDOW.GetWidth();
	frameBufferSpecification.Height = APPLICATION_WINDOW.GetHeight();
	m_FrameBuffer = Blind::FrameBuffer::Create(frameBufferSpecification);
}

void Sandbox2D::OnDetach()
{
	BL_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Blind::Timestep ts)
{
	BL_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(ts);

	{
		BL_PROFILE_SCOPE("Renderer Prep");
		m_FrameBuffer->Bind();
		Blind::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Blind::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += ts * 20.0f;

		BL_PROFILE_SCOPE("Renderer Draw");

		Blind::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Blind::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), { 0.2f, 0.8f, 0.4f, 1.0f });
		Blind::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Blind::Renderer2D::DrawQuad({ 0.f, 4.0f }, { 1.0f, 1.0f }, { 0.8f, 0.3, 0.3f, 1.0f });
		Blind::Renderer2D::DrawQuad({ 0.0, 1.0f }, { 1.0f, 3.0f }, m_SquareColor);
		Blind::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 15.0f, 15.0f }, m_CheckerboardTexture, 5.0f);

		Blind::Renderer2D::EndScene();

		Blind::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f};
				Blind::Renderer2D::DrawQuad({ x, y, }, {0.45f, 0.45f}, color);
			}
		}
		Blind::Renderer2D::EndScene();
		m_FrameBuffer->Unbind();
	}
}

void Sandbox2D::OnImGuiDraw()
{
	BL_PROFILE_FUNCTION();

	static bool dockingEnabled = true;

	if (dockingEnabled)
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit"))  APPLICATION.Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}
	
	ImGui::Begin("Settings");

	auto stats = Blind::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)textureID, ImVec2{1280.0f, 720.0f}, ImVec2{ 0,1 }, ImVec2{ 1,0 });

	ImGui::End();
	if(dockingEnabled)
		ImGui::End();
}

void Sandbox2D::OnEvent(Blind::Event& e)
{
	m_CameraController.OnEvent(e);
}
