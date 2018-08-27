#pragma once

#include <GL/glew.h>

class VertexBuffer
{
private:
	GLuint m_BufferID;
	GLenum m_Target;
	GLenum m_Usage;

public:
	VertexBuffer(GLenum Target, GLenum Usage);
	~VertexBuffer();

	void SetData(GLfloat *Data, GLsizei Count);

	void Bind() const;
	void Unbind() const;

};

