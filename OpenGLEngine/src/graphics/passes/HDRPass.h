#pragma once

#include <glm/glm.hpp>

#include "../../glsl/glsl.h"

#include "../textures/Texture2D.h"
#include "../textures/Sampler.h"

#include "../buffers/FrameBuffer.h"

class HDRPass
{
private:
	glShader *m_pShaderHDRPass;

	Texture2D *m_pHDRTexture;
	Texture2D *m_pBloomTexture;
	Sampler *m_pSampler;

	GLint m_ExposureLocation;

public:
	HDRPass(Texture2D *pHDRTexture, Texture2D *pBloomTexture);
	~HDRPass();

	bool Init(glShaderManager *pShaderManager);

	void Render();

	void Begin();

	void SetExposure(float exposure) const;

};