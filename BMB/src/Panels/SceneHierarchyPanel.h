#pragma once
#include <Blind/Scene/Scene.h>
#include <Blind/Core/Core.h>
#include <Blind/Scene/Entity.h>

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
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		Ref<Scene> m_Context; 
		Entity m_SelectionContext;

		friend class Scene;
	};
}