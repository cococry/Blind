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
	private:
		void DrawComponents(Entity entity);
		Ref<Scene> m_Context;
		Entity m_SelectionContext;

		friend class Scene;
	};
}