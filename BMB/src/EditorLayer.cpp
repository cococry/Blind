
#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Blind/Scene/SceneSerializer.h"
#include "Blind/Utils/PlatformUtils.h"
#include "Blind/Math/Mathematics.h"

#include "ImGuizmo.h"

namespace Blind
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		BL_PROFILE_FUNCTION();

		FrameBufferSpecification frameBufferSpecification;
		frameBufferSpecification.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::SHADER_RED_INT, FramebufferTextureFormat::Depth };
		frameBufferSpecification.Width = APPLICATION_WINDOW.GetWidth();
		frameBufferSpecification.Height = APPLICATION_WINDOW.GetHeight();
		m_FrameBuffer = FrameBuffer::Create(frameBufferSpecification);

		m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 1.778, 0.1f, 1000.0f);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		BL_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		BL_PROFILE_FUNCTION();

		if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Width != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		Renderer2D::ResetStats();
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		m_FrameBuffer->ClearAttachment(1, -1);

		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
		if (m_ViewportFocused)
		{
			m_EditorCamera.OnUpdate(ts);
		}

		auto [iMouseX, iMouseY] = ImGui::GetMousePos();
		iMouseX -= m_ViewportBounds[0].x;
		iMouseY -= m_ViewportBounds[0].y;
		
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		iMouseY = (viewportSize.y - iMouseY) - 26; // Making the bottom left 0, 0
		int mouseX = (int)iMouseX;
		int mouseY = (int)iMouseY;
		if (mouseX >= 0 && mouseY + 26 >= 0 && mouseX < (int)viewportSize.x && mouseY + 26 < (int)viewportSize.y)
		{
			int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY + 26);
			if (pixelData == -1)
				m_HoveredEntity = {};
			else
				m_HoveredEntity = { (entt::entity)pixelData, m_ActiveScene.get()};
		}

		m_FrameBuffer->Unbind();
	}

	void EditorLayer::OnImGuiDraw()
	{
		BL_PROFILE_FUNCTION();
		SetupImGuiDockspaceForDraw();
		m_SceneHierarchyPanel.OnImGuiDraw();
		HandleAndDrawMenuBar();
		DrawImGuiSettingsPanel();
		HandleImGuiViewport();
		HandleAndDrawImGuizmo();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FUNCTION(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUNCTION(EditorLayer::OnMouseButtonPressed));
	}
	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}
	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Blind Scene (*.blind)\0*.blind\0");
		if (!filepath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}
	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Blind Scene (*.blind)\0*.blind\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}
	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		//bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
		case Key::N:
		{
			if (control)
				NewScene();

			break;
		}
		case Key::O:
		{
			if (control)
				OpenScene();

			break;
		}
		case Key::S:
		{
			if (control)
				SaveSceneAs();

			break;
		}
		}
		switch (e.GetKeyCode())
		{
		case Key::Q:
		{
			m_GuizmoType = -1;
			break;
		}
		case Key::W:
			m_GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case Key::R:
			m_GuizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case Key::S:
			m_GuizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
	}
	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
			if(m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		return false;
	}
	void EditorLayer::SetupImGuiDockspaceForDraw()
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
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 400.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;

	}
	void EditorLayer::HandleAndDrawMenuBar()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					NewScene();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Save As...", "Ctrl+S"))
				{
					SaveSceneAs();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit"))  APPLICATION.Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

	}
	void EditorLayer::HandleAndDrawImGuizmo()
	{
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GuizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;
			if (m_GuizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GuizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}

		}
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}
	void EditorLayer::DrawImGuiSettingsPanel()
	{
		ImGui::Begin("Stats");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::Begin("Help");
		ImGui::Text("Camera Controller Input:");
		ImGui::Text("Pan: Ctrl + Middle-Mouse");
		ImGui::Text("Orbit: Ctrl + Left_Mouse");
		ImGui::Text("Zoom: Scrollwheel");
		ImGui::Text("Gizmos:");
		ImGui::Text("Move: W");
		ImGui::Text("Rotate: R");
		ImGui::Text("Scale: S");
		ImGui::Text("Disable: Q");
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	}
	void EditorLayer::HandleImGuiViewport()
	{
		ImGui::Begin("Viewport");
		ImVec2 viewportOffset = ImGui::GetCursorPos();

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		APPLICATION.GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize))
		{

			m_FrameBuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		// Calculating Viewport bounds
		ImVec2 windowSize = ImGui::GetWindowSize(); // Viewport
		ImVec2 minimumBounds = ImGui::GetWindowPos();
		minimumBounds.x += viewportOffset.x;
		minimumBounds.y += viewportOffset.y;

		ImVec2 maximumBounds = { minimumBounds.x + windowSize.x, minimumBounds.y + windowSize.y };
		m_ViewportBounds[0] = { minimumBounds.x, minimumBounds.y };
		m_ViewportBounds[1] = { maximumBounds.x, maximumBounds.y };

	}
}