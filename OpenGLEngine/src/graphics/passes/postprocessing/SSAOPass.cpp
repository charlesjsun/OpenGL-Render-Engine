#include "SSAOPass.h"

#include <glm/gtc/type_ptr.hpp>

#include "../../buffers/FrameBuffers.h"

#include "../../../Game.h"

#define KERNEL_SIZE 16
#define NUM_NOISE 16

SSAOPass::SSAOPass(Texture2D *pDepthTexture, Texture2D *pNormalTexture, Texture2D *pSSAOTexture)
	: m_pDepthTexture(pDepthTexture), m_pNormalTexture(pNormalTexture), m_pSSAOTexture(pSSAOTexture)
{
	
}

SSAOPass::~SSAOPass()
{
	delete m_pSampler;
	delete m_pTempSSAOTexture;
	delete m_pSSAOBlurPass;
	delete m_pSSAOFBO;
}

bool SSAOPass::Init(glShaderManager *pShaderManager, int width, int height)
{


	m_pTempSSAOTexture = new Texture2D(0, TEX_RED, width, height, NULL);
	m_pTempSSAOTexture->Init();

	m_pSSAOBlurPass = new BasicPostProcessingPass(m_pTempSSAOTexture, m_pSSAOTexture);
	if (!m_pSSAOBlurPass->Init(pShaderManager, POSTPROC_BOX_BLUR_VS, POSTPROC_BOX_BLUR_FS))
		return false;

	m_pShaderSSAOPass = pShaderManager->loadfromFile("src/graphics/shaders/postprocess/ssao.vs.glsl", "src/graphics/shaders/postprocess/ssao.fs.glsl");
	if (m_pShaderSSAOPass == NULL)
		return false;

	m_KernelLocation			= m_pShaderSSAOPass->GetUniformLocation("Kernel");
	m_NoiseLocation				= m_pShaderSSAOPass->GetUniformLocation("Noise");
	m_SampleRadiusLocation		= m_pShaderSSAOPass->GetUniformLocation("SampleRadius");
	m_NoiseScaleLocation		= m_pShaderSSAOPass->GetUniformLocation("NoiseScale");
	m_ProjectionMatrixLocation	= m_pShaderSSAOPass->GetUniformLocation("Projection");

	if (m_KernelLocation == -1 ||
		m_NoiseLocation	== -1 ||
		m_SampleRadiusLocation == -1 ||
		m_NoiseScaleLocation == -1 ||
		m_ProjectionMatrixLocation == -1
		)
		return false;

	m_pShaderSSAOPass->begin();
	SetKernal();
	SetNoise();
	m_pShaderSSAOPass->end();

	const GLuint samplerUnits[] = {
		m_pDepthTexture->GetTextureUnit(),
		m_pNormalTexture->GetTextureUnit()
	};
	m_pSampler = new Sampler(2, samplerUnits);
	m_pSampler->AddParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	m_pSampler->AddParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	m_pSampler->AddParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_pSampler->AddParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_pSampler->Init();

	m_pSSAOFBO = new FrameBuffer();
	m_pSSAOFBO->AttachTexture(m_pDepthTexture, GL_DEPTH_STENCIL_ATTACHMENT, false);
	m_pSSAOFBO->AttachTexture(m_pNormalTexture, GL_COLOR_ATTACHMENT0, false);
	m_pSSAOFBO->AttachTexture(m_pTempSSAOTexture, GL_COLOR_ATTACHMENT1, true);
	if (!m_pSSAOFBO->Init())
		return false;

	return true;
}

GLfloat SSAOPass::Lerp(GLfloat a, GLfloat b, GLfloat f)
{
	return a + f * (b - a);
}

void SSAOPass::SetKernal()
{
	glm::vec3 kernel[KERNEL_SIZE];

	for (GLuint i = 0; i < KERNEL_SIZE; i++) 
	{
		glm::vec3 sample(
			(float)rand() / RAND_MAX * 2.0 - 1.0,
			(float)rand() / RAND_MAX * 2.0 - 1.0, 
			(float)rand() / RAND_MAX);
		sample = glm::normalize(sample);
		sample *= (float)rand() / RAND_MAX;

		GLfloat scale = GLfloat(i) / KERNEL_SIZE;

		// Scale samples s.t. they're more aligned to center of kernel
		scale = Lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;

		kernel[i] = sample;
	}

	glUniform3fv(m_KernelLocation, KERNEL_SIZE, (const GLfloat*)&kernel[0]);

}

void SSAOPass::SetNoise()
{

	glm::vec3 ssaoNoise[NUM_NOISE];
	for (GLuint i = 0; i < NUM_NOISE; i++)
	{
		glm::vec3 noise(
			(float)rand() / RAND_MAX * 2.0 - 1.0,
			(float)rand() / RAND_MAX * 2.0 - 1.0,
			0.0f);
		ssaoNoise[i] = noise;
	}

	glUniform3fv(m_NoiseLocation, NUM_NOISE, (const GLfloat*)&ssaoNoise[0]);

}

void SSAOPass::Render()
{

	Begin();

	m_pSSAOFBO->Bind();

	m_pSampler->Bind();
	m_pDepthTexture->Bind();
	m_pNormalTexture->Bind();

	//m_pNoiseSampler->Bind();
	//m_pNoiseTexture->Bind();

	SetProjectionMatrix(Game::GetInstance()->m_pCamera->GetProjectionMatrix());

	glDisable(GL_CULL_FACE);

	Game::GetInstance()->m_pQuad->Render();

	m_pSSAOBlurPass->Render();

}

void SSAOPass::Begin()
{
	m_pShaderSSAOPass->begin();
}

void SSAOPass::SetSampleRadius(float sampleRadius) const
{
	glUniform1f(m_SampleRadiusLocation, sampleRadius);
}

void SSAOPass::SetProjectionMatrix(const glm::mat4& projectionMatrix) const
{
	glUniformMatrix4fv(m_ProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void SSAOPass::SetNoiseScale(float windowWidth, float windowHeight)
{
	//glUniform2f(m_NoiseScaleLocation, windowWidth / SSAO_NOISE_SIZE, windowHeight / SSAO_NOISE_SIZE);
	glUniform2f(m_NoiseScaleLocation, windowWidth, windowHeight);
}