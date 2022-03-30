#pragma once
#include <Blind/Renderer/VertexArray.h>
namespace Blind
{
	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(RendererID vao, uint32_t vertexCount)
			: m_VAO(vao), m_VertexCount(vertexCount)
		{

		}
		inline RendererID GetVaoID() const { return m_VAO; }
		inline int32_t GetVertexCount() const { return m_VertexCount; }
	private:
		RendererID m_VAO;
		uint32_t m_VertexCount;
	};
}