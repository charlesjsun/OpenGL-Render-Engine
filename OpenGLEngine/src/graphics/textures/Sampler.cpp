#include "Sampler.h"

Sampler::Sampler(GLuint amount, const GLuint units[])
{
	
	for (int i = 0; i < amount; i++)
		m_TextureUnits.push_back(units[i]);
}

Sampler::~Sampler()
{
	
	glDeleteSamplers(1, &m_SamplerID);

}

void Sampler::AddParameter(GLenum pname, GLint param)
{
	m_Parameters[pname] = param;
}

bool Sampler::Init()
{

	glGenSamplers(1, &m_SamplerID);

	for (auto const& param : m_Parameters)
		glSamplerParameteri(m_SamplerID, param.first, param.second);

	return true;

}

void Sampler::Bind() const
{
	for (const GLint& unit : m_TextureUnits)
		glBindSampler(unit, m_SamplerID);
}

void Sampler::Unbind() const
{
	for (const GLint& unit : m_TextureUnits)
		glBindSampler(unit, 0);
}