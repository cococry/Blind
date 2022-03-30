#pragma once
#pragma once
#include <Blind/Core/Core.h>
#include "Mesh.h"
#include <vector>
namespace Blind
{
	class MeshLoader
	{
	public:
		Ref<Mesh> LoadToVAO(const std::vector<float>& vertices, const std::vector<uint32_t>& indices, float dimensions = 3);

		void DeleteData();
	private:
		void SetLayoutAttribute(int32_t attribute, const std::vector<float>& data, float dimensions = 3);
		void UnbindVAO();
		RendererID CreateVAO();
		void SetEBO(const std::vector<uint32_t>& indices);

		std::vector<uint32_t> m_VAOs, m_VBOs;
	};

}
