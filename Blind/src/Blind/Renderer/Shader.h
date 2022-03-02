#pragma once

#include <string>

namespace Blind
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind();
		void Unbind();

		inline uint32_t GetID() const { return m_RendererID; }
		inline bool Bound() const { return m_Bound; }
	private:
		uint32_t m_RendererID;
		bool m_Bound;
	};
}