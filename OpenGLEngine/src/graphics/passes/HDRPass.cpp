#include "HDRPass.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Game.h"


HDRPass::HDRPass(Texture2D *pHDRTexture, Texture2D *pBloomTexture)
	:m_pHDRTexture(pHDRTexture), m_pBloomTexture(pBloomTexture)
{

}

HDRPass::~HDRPass()
{
	delete m_pSampler;
}

bool HDRPass::Init(glShaderManager *pShaderManager)
{

	m_pShaderHDRPass = pShaderManager->loadfromFile("src/graphics/shaders/hdr_pass.vs.glsl", "src/graphics/shaders/hdr_pass.fs.glsl");
	if (m_pShaderHDRPass == NULL)
		return false;

	m_ExposureLocation = m_pShaderHDRPass->GetUniformLocation("Exposure");

	if (m_ExposureLocation == -1
		)
		return false;

	const GLuint samplerUnits[] = { 
		m_pHDRTexture->GetTextureUnit(),
		m_pBloomTexture->GetTextureUnit()
	};
	m_pSampler = new Sampler(2, samplerUnits);
	m_pSampler->AddParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	m_pSampler->AddParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	m_pSampler->AddParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_pSampler->AddParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_pSampler->Init();

	return true;
}

void HDRPass::Render()
{
	Begin();

	FrameBuffer::BindDefaultFrameBuffer();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pSampler->Bind();
	m_pHDRTexture->Bind();
	m_pBloomTexture->Bind();

	Game::GetInstance()->m_pQuad->Render();

}

void HDRPass::Begin()
{
	m_pShaderHDRPass->begin();
	
}

void HDRPass::SetExposure(float exposure) const
{
	glUniform1f(m_ExposureLocation, exposure);
}