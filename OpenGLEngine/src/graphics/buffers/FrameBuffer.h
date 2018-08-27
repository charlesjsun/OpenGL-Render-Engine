#pragma once

#include <GL/glew.h>
#include <map>
#include <vector>

#include "../textures/Texture2D.h"

class FrameBuffer
{
private:
	GLuint m_BufferID = -1;

	std::map<GLenum, const Texture2D *> m_Textures;
	std::map<GLenum, GLint> m_Parameters;
	std::vector<GLenum> m_DrawTextures;
	
public:
	FrameBuffer();
	~FrameBuffer();

	void AttachTexture(const Texture2D *pTexture, GLenum attachment, bool drawToTexture);
	void AddParameter(GLenum pname, GLint param);
	bool Init();

	void Bind() const;
	void BindRead() const;
	void BindDraw() const;

	static void BindDefaultFrameBuffer();
	static void BindDefaultAsRead();
	static void BindDefaultAsDraw();

};