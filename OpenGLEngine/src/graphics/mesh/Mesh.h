#pragma once

#include "../buffers/VertexArray.h"
#include "../buffers/IndexBuffer.h"

#define VERTEX_POSITION_ATTRIB	0
#define VERTEX_NORMAL_ATTRIB	1
#define VERTEX_COLOR_ATTRIB		2
#define VERTEX_TEXTURE_ATTRIB	2

class Mesh
{
private:
	VertexArray *m_pVAO;
	IndexBuffer *m_pIBO;

public:
	Mesh(std::vector<GLfloat>& Vertices, std::vector<GLuint>& Indices, std::vector<VertexAttribute>& Attributes);
	~Mesh();

	void Render(GLenum RenderMode = GL_TRIANGLES) const;

};