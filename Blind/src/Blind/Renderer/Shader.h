#pragma once

#include <string>
#include <Blind/Core.h>
#include <unordered_map>

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
