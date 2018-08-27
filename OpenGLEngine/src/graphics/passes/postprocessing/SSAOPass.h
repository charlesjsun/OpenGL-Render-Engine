#pragma once

#include <glm/glm.hpp>

#include "../../../glsl/glsl.h"

#include "../../textures/Texture2D.h"
#include "../../textures/Sampler.h"

#include "BasicPostProcessingPass.h"

#include "../../buffers/FrameBuffer.h"

class SSAOPass
{
private:
	BasicPostProcessingPass *m_pSSAOBlurPass;
	Texture2D *m_pTempSSAOTexture;

	glShader *m_pShaderSSAOPass;

	Sampler *m_pSampler;
	Texture2D *m_pDepthTexture;
	Texture2D *m_pNormalTexture;
	Texture2D *m_pSSAOTexture;
	FrameBuffer *m_pSSAOFBO;

	GLint m_KernelLocation;
	GLint m_NoiseLocation;
	GLint m_SampleRadiusLocation;
	GLint m_ProjectionMatrixLocation;
	GLint m_NoiseScaleLocation;

public:
	SSAOPass(Texture2D *pDepthTexture, Texture2D *pNormalTexture, Texture2D *pSSAOTexture);
	~SSAOPass();

	bool Init(glShaderManager *pShaderManager, int width, int height);

	void Render();

	void Begin();

	void SetSampleRadius(float sampleRadius) const;
	void SetProjectionMatrix(const glm::mat4& projectionMatrix) const;

	void SetNoiseScale(float windowWidth, float windowHeight);

private:
	GLfloat Lerp(GLfloat a, GLfloat b, GLfloat f);
	void SetKernal();
	void SetNoise();

};