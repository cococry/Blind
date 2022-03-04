#pragma once

#include <Blind/Renderer/RendererAPI.h>

namespace Blind
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;

		virtual void Init() override;

	private:
	};
}