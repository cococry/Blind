#pragma once

#include <Blind/Renderer/FrameBuffer.h>

namespace Blind
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentI, int x, int y) override;

		virtual const FrameBufferSpecification& GetSpecification() const override { return m_Specification; };
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { BLIND_ENGINE_ASSERT(index < m_ColorAttachments.size(), "Invalid index!"); return m_ColorAttachments[index]; };

	private:
		uint32_t m_RendererID = 0;

		FrameBufferSpecification m_Specification;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		FramebufferTextureSpecification m_DepthAttachmentSpec = FramebufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};
}