#include "VertexArray.h"


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_ArrayID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ArrayID);
	delete m_pBuffer;
}

void VertexArray::AddVertexAttributePointer(const VertexAttribute& Attribute​) const
{
	glEnableVertexAttribArray(Attribute​.Index);
	glVertexAttribPointer(Attribute​.Index, Attribute​.Size, Attribute​.Type, Attribute​.Normalized, Attribute​.Stride * sizeof(Attribute​.Type), (GLvoid*)(Attribute​.Pointer * sizeof(Attribute​.Type)));
}

void  VertexArray::SetVertexBuffer(VertexBuffer *pVBO)
{
	m_pBuffer = pVBO;
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_ArrayID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

