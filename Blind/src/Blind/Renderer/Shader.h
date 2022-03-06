#pragma once

#include <string>
#include <Blind/Core/Core.h>
#include <unordered_map>
#include <glm/glm.hpp>

namespace Blind
{
	class Shader
	{
	public:
		~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual inline uint32_t GetID() const = 0;

		virtual const std::string& GetName() const = 0;

		virtual void SetMat4(const std::string& name, glm::mat4 value) = 0;
		virtual void SetMat3(const std::string& name, glm::mat3 value) = 0;
		virtual void SetVec4(const std::string& name, glm::vec4 value) = 0;
		virtual void SetVec3(const std::string& name, glm::vec3 value) = 0;
		virtual void SetVec2(const std::string& name, glm::vec2 value) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;

		static Ref<Shader> Create(const std::string& name, std::string& vertexSource, const std::string& fragmentSource);
		static Ref<Shader> Create(const std::string& filepath);
	};
	
	class ShaderLibrary
	{
	public: 
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}
