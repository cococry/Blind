#pragma once
#include <Blind/Scene/Scene.h>
#include <Blind/Core/Core.h>
#include <Blind/Scene/Entity.h>
#include <Blind/Renderer/Texture.h>
namespace Blind
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiDraw();
		
		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; };

		void ClearScene();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		Ref<Scene> m_Context; 
		Ref<Texture2D> m_CheckerboardTexture;
		Entity m_SelectionContext;

		friend class Scene;
	};
}