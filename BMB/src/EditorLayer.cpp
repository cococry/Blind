
#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Blind
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController((float)APPLICATION_WINDOW.GetWidth() / (float)APPLICATION_WINDOW.GetHeight())
	{
	}

	void EditorLayer::OnAttach()
	{
		BL_PROFILE_FUNCTION();

		m_CheckerboardTexture = Blind::Texture2D::Create("assets/textures/checkerboard.png");

		m_CameraController.SetZoomLevel(5.0f);

		Blind::FrameBufferSpecification frameBufferSpecification;
		frameBufferSpecification.Width = APPLICATION_WINDOW.GetWidth();
		frameBufferSpecification.Height = APPLICATION_WINDOW.GetHeight();
		m_FrameBuffer = Blind::FrameBuffer::Create(frameBufferSpecification);
	}

	void EditorLayer::OnDetach()
	{
		BL_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Blind::Timestep ts)
	{
		BL_PROFILE_FUNCTION();

		if(m_ViewportFocused)
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
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
					Blind::Renderer2D::DrawQuad({ x, y, }, { 0.45f, 0.45f }, color);
				}
			}
			Blind::Renderer2D::EndScene();
			m_FrameBuffer->Unbind();
		}
	}

	void EditorLayer::OnImGuiDraw()
	{
		BL_PROFILE_FUNCTION();
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

		ImGui::Begin("Settings");

		auto stats = Blind::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::ColorEdit4("Quad Color", glm::value_ptr(m_SquareColor));
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		APPLICATION.GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{
			m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);

			m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Blind::Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}