#pragma once

#include <GL/glew.h>
#include <vector>
#include <map>

class Sampler
{
private:
	GLuint m_SamplerID = -1;

	std::vector<GLuint> m_TextureUnits;
	std::map<GLenum, GLint> m_Parameters;
	
public:
	Sampler(GLuint amount, const GLuint units[]);
	~Sampler();

	void AddParameter(GLenum pname, GLint param);

	bool Init();

	void Bind() const;
	void Unbind() const;

};