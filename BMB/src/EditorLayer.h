#pragma once

#include <BlindEngine.h>
#include "Panels/SceneHierarchyPanel.h" 
#include "Blind/Renderer/EditorCamera.h" 

namespace Blind
{
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
		void SaveSceneAs();

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void SetupImGuiDockspaceForDraw();
		void HandleAndDrawMenuBar();
		void HandleAndDrawImGuizmo();
		void DrawImGuiSettingsPanel();
		void HandleImGuiViewport();

		Ref<Shader> m_FlatColorShader;
		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_HoveredEntity;

		glm::vec2 m_ViewportSize = {0.0f, 0.0f};
		glm::vec2 m_ViewportBounds[2];
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		bool m_PrimaryCamera   = true;

		int32_t m_GuizmoType = -1;

		SceneHierarchyPanel m_SceneHierarchyPanel;
		EditorCamera m_EditorCamera;

	};
}
