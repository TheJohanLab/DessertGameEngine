#include "dgepch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Dessert {



	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererId);
		vertexBuffer->Bind();
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
	}

}