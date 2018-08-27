#pragma once

#define TEXTURE_UNIT_DEPTH		0
#define TEXTURE_UNIT_NORMAL		1
#define TEXTURE_UNIT_COLOR		2
#define TEXTURE_UNIT_HDR		3
#define TEXTURE_UNIT_SSAO		4
#define TEXTURE_UNIT_MATERIALS	5
#define TEXTURE_UNIT_BLOOM		6


enum TextureFormat
{
	TEX_DEPTH,
	TEX_STENCIL,
	TEX_DEPTH_STENCIL,
	TEX_RED,
	TEX_RGB,
	TEX_RGBA,
	TEX_RGB16F,
	TEX_RGBA16F,
	TEX_RGB32F,
	TEX_RGBA32F
};

class Texture
{
protected:
	GLuint m_TextureID = -1;

	GLuint m_TextureUnit;
	GLint m_InternalFormat;
	GLsizei m_Width;
	GLsizei m_Height;
	GLenum m_Format;
	GLenum m_Type;
	const void *m_Pixels;

protected:
	Texture(GLuint textureUnit, TextureFormat format, GLsizei width, GLsizei height, const void *pixels)
	{
		m_TextureUnit = textureUnit;
		m_Width = width;
		m_Height = height;
		m_Pixels = pixels;

		switch (format)
		{
		case TEX_DEPTH:
			m_InternalFormat = GL_DEPTH_COMPONENT24;
			m_Format = GL_DEPTH_COMPONENT;
			m_Type = GL_UNSIGNED_INT;
			break;
		case TEX_STENCIL:
			m_InternalFormat = GL_STENCIL_INDEX8;
			m_Format = GL_STENCIL_INDEX;
			m_Type = GL_UNSIGNED_BYTE;
			break;
		case TEX_DEPTH_STENCIL:
			m_InternalFormat = GL_DEPTH24_STENCIL8;
			m_Format = GL_DEPTH_STENCIL;
			m_Type = GL_UNSIGNED_INT_24_8;
			break;
		case TEX_RED:
			m_InternalFormat = GL_RED;
			m_Format = GL_RED;
			m_Type = GL_FLOAT;
			break;
		case TEX_RGB:
			m_InternalFormat = GL_RGB;
			m_Format = GL_RGB;
			m_Type = GL_FLOAT;
			break;
		case TEX_RGBA:
			m_InternalFormat = GL_RGBA;
			m_Format = GL_RGBA;
			m_Type = GL_FLOAT;
			break;
		case TEX_RGB16F:
			m_InternalFormat = GL_RGB16F;
			m_Format = GL_RGB;
			m_Type = GL_FLOAT;
			break;
		case TEX_RGBA16F:
			m_InternalFormat = GL_RGBA16F;
			m_Format = GL_RGBA;
			m_Type = GL_FLOAT;
			break;
		case TEX_RGB32F:
			m_InternalFormat = GL_RGB32F;
			m_Format = GL_RGB;
			m_Type = GL_FLOAT;
			break;
		case TEX_RGBA32F:
			m_InternalFormat = GL_RGBA32F;
			m_Format = GL_RGBA;
			m_Type = GL_FLOAT;
			break;
		}
	}

	virtual ~Texture()
	{
		if (m_TextureID != -1)
			glDeleteTextures(1, &m_TextureID);
	}

public:

	virtual bool Init() = 0;
	virtual void Bind() const = 0;
	virtual void Bind(GLuint unit) const = 0;

	inline GLuint GetTextureUnit() const { return m_TextureUnit; }
	inline GLsizei GetWidth() const { return m_Width; }
	inline GLsizei GetHeight() const { return m_Height; }


};