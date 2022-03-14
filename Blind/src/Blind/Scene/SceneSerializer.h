#pragma once

#include "Scene.h"
#include <Blind/Core/Core.h>

namespace Blind
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);
	private:
		Ref<Scene> m_Scene;
	};
}