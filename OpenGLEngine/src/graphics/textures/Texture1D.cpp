#include "Texture1D.h"

Texture1D::Texture1D(GLuint textureUnit, TextureFormat format, GLsizei width, const void *pixels)
	: Texture(textureUnit, format, width, 0, pixels)
{

}

Texture1D::~Texture1D()
{

}

bool Texture1D::Init()
{

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_1D, m_TextureID);
	glTexImage1D(GL_TEXTURE_1D, 0, m_InternalFormat, m_Width, 0, m_Format, m_Type, m_Pixels);
	glBindTexture(GL_TEXTURE_1D, 0);

	return true;
}

void Texture1D::Bind() const
{
	glActiveTexture(GL_TEXTURE0 + m_TextureUnit);
	glBindTexture(GL_TEXTURE_1D, m_TextureID);
}

void Texture1D::Bind(GLuint unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_1D, m_TextureID);
}