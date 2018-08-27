#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(GLuint *Data, GLsizei Count)
{
	m_Count = Count;

	glGenBuffers(1, &m_BufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Count * sizeof(GLuint), Data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_BufferID);
}


void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

