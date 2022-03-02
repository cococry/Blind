#include "blindpch.h"
#include "OpenGLBuffer.h"
#include <Blind/Log.h>

#include <glad/glad.h>

namespace Blind
{
	////////////////////////////////////////////////////////////////////////////////////
	//// VERTEX BUFFER /////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

		m_Bound = true;
	}
	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}
	void OpenGLVertexBuffer::Bind()
	{
		if (!m_Bound)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
			m_Bound = true;
		}
		else
			BLIND_ENGINE_WARN("Tried to bind already bound Vertex Buffer ({0}).", m_RendererID);
			
	}
	void OpenGLVertexBuffer::Unbind()
	{
		if (m_Bound)
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_Bound = false;
		}
		else
			BLIND_ENGINE_WARN("Tried to unbind not bound Vertex Buffer ({0}).", m_RendererID);
	}

	////////////////////////////////////////////////////////////////////////////////////
	//// INDEX BUFFER /////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////


	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

		m_Bound = true;
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind()
	{
		if (!m_Bound)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
			m_Bound = true;
		}
		else
			BLIND_ENGINE_WARN("Tried to bind already bound Index Buffer ({0}).", m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind()
	{
		if (m_Bound)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			m_Bound = false;
		}
		else
			BLIND_ENGINE_WARN("Tried to unbind not bound Index Buffer ({0}).", m_RendererID);
	}

}