#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(GLenum Target, GLenum Usage)
	: m_Target(Target), m_Usage(Usage)
{
	glGenBuffers(1, &m_BufferID);
	
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_BufferID);
}

void VertexBuffer::SetData(GLfloat *Data, GLsizei Count)
{
	glBufferData(m_Target, Count * sizeof(GLfloat), Data, m_Usage);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(m_Target, m_BufferID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(m_Target, 0);
}

