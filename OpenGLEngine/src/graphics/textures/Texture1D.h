#pragma once

#include <GL/glew.h>

#include "Texture.h"

class Texture1D : public Texture
{
private:


public:
	Texture1D(GLuint textureUnit, TextureFormat format, GLsizei width, const void *pixels);
	~Texture1D();

	bool Init() override;
	void Bind() const override;
	void Bind(GLuint unit) const override;

};