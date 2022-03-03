#pragma once
#include <string>
#include "RenderCommand.h"

#include "OrthographicCamera.h"
#include "Shader.h"


namespace Blind
{
	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		static std::string RendererAPIToString();
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* m_SceneData;
	};
}