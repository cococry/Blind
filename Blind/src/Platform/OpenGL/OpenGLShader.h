#pragma once

#include <Blind/Renderer/Shader.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

typedef unsigned int GLenum;

namespace Blind
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
		OpenGLShader(const std::string& filepath);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; }

		void UploadMat4(const std::string& name, glm::mat4 val);
		void UploadMat3(const std::string& name, glm::mat4 val);
		void UploadVec4(const std::string& name, glm::vec4 val);
		void UploadVec3(const std::string& name, glm::vec3 val);
		void UploadVec2(const std::string& name, glm::vec2 val);
		void UploadFloat(const std::string& name, float val);
		void UploadInt(const std::string& name, int val);

		virtual void SetMat4(const std::string& name, glm::mat4 value) override;
		virtual void SetMat3(const std::string& name, glm::mat3 value) override;
		virtual void SetVec4(const std::string& name, glm::vec4 value) override;
		virtual void SetVec3(const std::string& name, glm::vec3 value) override;
		virtual void SetVec2(const std::string& name, glm::vec2 value) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetInt(const std::string& name, int value) override;

		virtual inline uint32_t GetID() const override { return m_RendererID; }
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

		uint32_t m_RendererID;
		std::string m_Name;
	};
}