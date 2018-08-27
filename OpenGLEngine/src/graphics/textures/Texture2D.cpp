#include "Texture2D.h"

Texture2D::Texture2D(GLuint textureUnit, TextureFormat format, GLsizei width, GLsizei height, const void *pixels)
	: Texture(textureUnit, format, width, height, pixels)
{

}

Texture2D::~Texture2D()
{
	
}

bool Texture2D::Init()
{

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, m_Type, m_Pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void Texture2D::Bind() const
{
	glActiveTexture(GL_TEXTURE0 + m_TextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture2D::Bind(GLuint unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture2D::AttachToBoundFBO(GLenum attachment) const
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_TextureID, 0);
}