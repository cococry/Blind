#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Blind
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind();
		void Unbind();

		void UploadMat4(const std::string& name, glm::mat4 val);
		void UploadMat3(const std::string& name, glm::mat4 val);
		void UploadVec4(const std::string& name, glm::vec4 val);
		void UploadVec3(const std::string& name, glm::vec3 val);
		void UploadVec2(const std::string& name, glm::vec2 val);
		void UploadFloat(const std::string& name, float val);
		void UploadInt(const std::string& name, int val);

		inline uint32_t GetID() const { return m_RendererID; }
	private:
		uint32_t m_RendererID;
	};
}