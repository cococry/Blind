#include "blindpch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>
#include <Blind/Core/Log.h>

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
		BL_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		BL_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
		BLIND_ENGINE_ASSERT(m_RendererID, "Failed to create Vertex Array!");
		BLIND_ENGINE_TRACE("Created Vertex Array ({0}) successfully.", m_RendererID);
	}
	void OpenGLVertexArray::Bind()
	{
		BL_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind()
	{
		BL_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		BL_PROFILE_FUNCTION();

		BLIND_ENGINE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no Layout. Use VertexBuffer.SetLayout(...)!");
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset));
					index++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribIPointer(index,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						layout.GetStride(),
						(const void*)(element.Offset));
					index++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t count = element.GetComponentCount();
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(index);
						glVertexAttribIPointer(index,
							count,
							ShaderDataTypeToOpenGLBaseType(element.Type),
							layout.GetStride(),
							(const void*)(element.Offset + sizeof(float) * count * i));
						glVertexAttribDivisor(index, 1);
						index++;
					}
					break;
				}
			}

			BLIND_ENGINE_TRACE("Added Vertex buffer layout element '{0} with properties: (element index: {1}, vertex stride: {2}, element offset: {3}) to Vertex Buffer ({4}).",
				element.Name, index, layout.GetStride(), element.Offset, vertexBuffer->GetID());
		}
		m_VertexBuffers.push_back(vertexBuffer);
		BLIND_ENGINE_TRACE("Added Vertex Buffer ({0}) to Vertex Array ({1}).", vertexBuffer->GetID(), m_RendererID);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		BL_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;

		BLIND_ENGINE_TRACE("Added Index Buffer ({0}) to Vertex Array ({1}).", indexBuffer->GetID(), m_RendererID);
	}
}
