#pragma once

#include <Blind/Renderer/VertexArray.h>

namespace Blind
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		virtual ~OpenGLVertexArray();
		OpenGLVertexArray();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		inline uint32_t GetID() const override { return m_RendererID; }


		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffs() const override { return m_VertexBuffers; };
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; };
	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer>  m_IndexBuffer;

		uint32_t m_RendererID;
	};
}