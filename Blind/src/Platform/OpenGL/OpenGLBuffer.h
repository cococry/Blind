#pragma once

#include <Blind/Renderer/Buffer.h>

namespace Blind
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		inline uint32_t GetID() const { return m_RendererID; }
		inline bool Bound() const { return m_Bound; }

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		bool m_Bound;
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		inline uint32_t GetID() const { return m_RendererID; }
		virtual uint32_t GetCount() const override { return m_Count; }
		inline bool Bound() const { return m_Bound; }
	private:
		bool m_Bound;
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}