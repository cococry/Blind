#pragma once

#include <BlindEngine.h>
#include "Panels/SceneHierarchyPanel.h" 
#include "Panels/ContentBrowserPanel.h" 
#include "Blind/Renderer/EditorCamera.h" 

namespace Blind
{
	enum class SceneState
	{
		Edit = 0,
		Play = 1
	};
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() {};

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiDraw() override;
		virtual void OnEvent(Event& e) override;
	private:
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveSceneAs();
		void SaveScene();

		void DrawUI();

		void OnScenePlay();
		void OnSceneStop();

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void SetupImGuiDockspaceForDraw();
		void HandleAndDrawMenuBar();
		void HandleAndDrawImGuizmo();
		void DrawImGuiSettingsPanel();
		void HandleImGuiViewport();

		template<typename UIFunction>
		void BeginPopup(const char* name, UIFunction UIfunc)
		{
			bool running = true;
	
			Util::BeginCentered(name);
			UIfunc();

			if(ImGui::Button("OK", ImVec2(200, 200))) {
				running = false;
			}
			
		}

		Ref<Shader> m_FlatColorShader;
		Ref<FrameBuffer> m_FrameBuffer;
		Ref<Texture2D> m_StartIcon;
		Ref<Texture2D> m_StopIcon;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene, m_RuntimeScene;

		Entity m_HoveredEntity;
		SceneState m_SceneState = SceneState::Edit;

		glm::vec2 m_ViewportSize = {0.0f, 0.0f};
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		bool m_PrimaryCamera   = true;
		bool m_ErrorPopupOpened = false;

		int32_t m_GuizmoType = -1;

		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		EditorCamera m_EditorCamera;

		std::filesystem::path m_EditorScenePath;

	};

}
