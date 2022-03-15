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
		OrthographicCameraController m_CameraController;

		Ref<Shader> m_FlatColorShader;
		Ref<VertexArray> m_SquareVA;
		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		Ref<Texture2D> m_CheckerboardTexture;

		glm::vec2 m_ViewportSize = {0.0f, 0.0f};
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		bool m_PrimaryCamera   = true;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		int32_t m_GuizmoType = -1;

		SceneHierarchyPanel m_SceneHierarchyPanel;
		EditorCamera m_EditorCamera;

	};
}
