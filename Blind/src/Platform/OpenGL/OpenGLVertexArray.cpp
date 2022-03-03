#include "blindpch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>
#include <Blind/Log.h>

namespace Blind
{

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Blind::ShaderDataType::Float:		return GL_FLOAT;
			case Blind::ShaderDataType::Float2:		return GL_FLOAT;
			case Blind::ShaderDataType::Float3:		return GL_FLOAT;
			case Blind::ShaderDataType::Float4:		return GL_FLOAT;
			case Blind::ShaderDataType::Mat3:		return GL_FLOAT;
			case Blind::ShaderDataType::Mat4:		return GL_FLOAT;
			case Blind::ShaderDataType::Int:		return GL_INT;
			case Blind::ShaderDataType::Int2:		return GL_INT;
			case Blind::ShaderDataType::Int3:		return GL_INT;
			case Blind::ShaderDataType::Int4:		return GL_INT;
			case Blind::ShaderDataType::Bool:		return GL_BOOL;
		}
		BLIND_ENGINE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind()
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind()
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		BLIND_ENGINE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no Layout. Use VertexBuffer.SetLayout(...)!");
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		BLIND_ENGINE_TRACE("Added Vertex Buffer ({0}) to Vertex Array ({1}).", vertexBuffer->GetID(), m_RendererID);

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(void*)(element.Offset));

			BLIND_ENGINE_TRACE("Added Vertex buffer layout element '{0} with properties: (element index: {1}, vertex stride: {2}, element offset: {3}).",
				element.Name, index, layout.GetStride(), element.Offset);

			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;

		BLIND_ENGINE_TRACE("Added Index Buffer ({0}) to Vertex Array ({1}).", indexBuffer->GetID(), m_RendererID);
	}
}
