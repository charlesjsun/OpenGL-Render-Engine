#pragma once

#include <GL/glew.h>

#include "Texture.h"

class Texture2D : public Texture
{
private:
	

public:
	Texture2D(GLuint textureUnit, TextureFormat format, GLsizei width, GLsizei height, const void *pixels);
	~Texture2D();

	void AttachToBoundFBO(GLenum attachment) const;

	bool Init() override;
	void Bind() const override;
	void Bind(GLuint unit) const override;
	
};