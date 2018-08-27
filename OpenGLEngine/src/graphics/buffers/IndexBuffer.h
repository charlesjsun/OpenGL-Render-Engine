#pragma once

#include <GL/glew.h>

class IndexBuffer
{
private:
	GLuint m_BufferID;
	GLuint m_Count;

public:
	IndexBuffer(GLuint *Data, GLsizei Count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline GLuint GetCount() const { return m_Count; };

};

