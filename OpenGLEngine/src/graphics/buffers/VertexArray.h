#pragma once

#include <GL/glew.h>
#include <vector>

#include "VertexBuffer.h"
#include "VertexAttribute.h"

class VertexArray
{
private:
	VertexBuffer *m_pBuffer;
	GLuint m_ArrayID;

public:
	VertexArray();
	~VertexArray();

	void AddVertexAttributePointer(const VertexAttribute& Attribute​) const;
	void SetVertexBuffer(VertexBuffer *pVBO);
	
	void Bind() const;
	void Unbind() const;

};
