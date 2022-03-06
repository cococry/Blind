#include "blindpch.h"
#include "OpenGLShader.h"
#include <Blind/Core/Log.h>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>

namespace Blind
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		BLIND_ENGINE_ASSERT(false, "Unknown Shader Type!");
		return 0;
	}
	static std::string ShaderTypeToString(GLenum shaderType)
	{
		if (shaderType == GL_FRAGMENT_SHADER) return "Fragment";
		if (shaderType == GL_VERTEX_SHADER) return "Vertex";

		BLIND_ENGINE_ASSERT(false, "Unknown Shader Type!");
		return 0;
	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSource;
		sources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(sources);
		BLIND_ENGINE_INFO("Created OpenGL Shader '{0}' successfully.", m_Name);
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		BL_PROFILE_FUNCTION();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind(".");

		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;

		m_Name = filepath.substr(lastSlash, count);
		BLIND_ENGINE_INFO("Created OpenGL Shader '{0}' successfully.", m_Name);
	}

	void OpenGLShader::UploadMat4(const std::string& name, glm::mat4 val)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(val));
	}

	void OpenGLShader::UploadMat3(const std::string& name, glm::mat4 val)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, false, glm::value_ptr(val));
	}

	void OpenGLShader::UploadVec4(const std::string& name, glm::vec4 val)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4fv(location, 1, glm::value_ptr(val));
	}

	void OpenGLShader::UploadVec3(const std::string& name, glm::vec3 val)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(val));
	}

	void OpenGLShader::UploadVec2(const std::string& name, glm::vec2 val)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2fv(location, 1, glm::value_ptr(val));
	}

	void OpenGLShader::UploadFloat(const std::string& name, float val)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, val);
	}

	void OpenGLShader::UploadInt(const std::string& name, int val)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, val);
	}

	void OpenGLShader::UploadIntArray(const std::string& name, int* values, uint32_t count)
	{
		int32_t location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::SetMat4(const std::string& name, glm::mat4 value)
	{
		BL_PROFILE_FUNCTION();

		UploadMat4(name, value);
	}

	void OpenGLShader::SetMat3(const std::string& name, glm::mat3 value)
	{
		BL_PROFILE_FUNCTION();

		UploadMat3(name, value);
	}

	void OpenGLShader::SetVec4(const std::string& name, glm::vec4 value)
	{
		BL_PROFILE_FUNCTION();

		UploadVec4(name, value);
	}

	void OpenGLShader::SetVec3(const std::string& name, glm::vec3 value)
	{
		BL_PROFILE_FUNCTION();

		UploadVec3(name, value);
	}

	void OpenGLShader::SetVec2(const std::string& name, glm::vec2 value)
	{
		BL_PROFILE_FUNCTION();

		UploadVec2(name, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		BL_PROFILE_FUNCTION();

		UploadFloat(name, value);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		BL_PROFILE_FUNCTION();

		UploadInt(name, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		BL_PROFILE_FUNCTION();

		UploadIntArray(name, values, count);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		BL_PROFILE_FUNCTION();
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			BLIND_ENGINE_ERROR("Could not open shader file '{0}'", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		BL_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			BLIND_ENGINE_ASSERT(eol != std::string::npos, "Syntax Error!");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			BLIND_ENGINE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specification.");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] =
				source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		BL_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		BLIND_ENGINE_ASSERT(shaderSources.size() <= 2, "Too many shaders specified. Only 2 Shaders are supported.");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;
			
			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				BLIND_ENGINE_ERROR("{0} Shader Compilation Failed!", ShaderTypeToString(type));
				BLIND_ENGINE_TRACE("{0}", infoLog.data());
				break;
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			 
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			for(auto id : glShaderIDs)
				glDeleteShader(id);

			BLIND_ENGINE_ERROR("{0} Shader Program Linking Failed!");
			BLIND_ENGINE_TRACE("{0}", infoLog.data());

			return;
		}

		for(auto id : glShaderIDs)
			glDetachShader(program, id);
		m_RendererID = program;
	}

	OpenGLShader::~OpenGLShader()
	{
		BL_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		BL_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		BL_PROFILE_FUNCTION();

		glUseProgram(0);
	}

}
