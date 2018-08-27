#include "BasicPostProcessingPass.h"

#include "../../../Game.h"

BasicPostProcessingPass::BasicPostProcessingPass(Texture2D *pInputTexture, Texture2D *pOutputTexture)
	: m_pInputTexture(pInputTexture), m_pOutputTexture(pOutputTexture)
{

}

BasicPostProcessingPass::~BasicPostProcessingPass()
{
	delete m_pSampler;
	delete m_pFBO;
}

bool BasicPostProcessingPass::Init(glShaderManager *pShaderManager, char *vs, char *fs)
{

	m_pShader = pShaderManager->loadfromFile(vs, fs);
	if (m_pShader == NULL)
		return false;

	const GLuint samplerUnits[] = { 0 };
	m_pSampler = new Sampler(1, samplerUnits);
	m_pSampler->AddParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	m_pSampler->AddParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	m_pSampler->AddParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_pSampler->AddParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_pSampler->Init();

	m_pFBO = new FrameBuffer();
	m_pFBO->AttachTexture(m_pInputTexture, GL_COLOR_ATTACHMENT0, false);
	m_pFBO->AttachTexture(m_pOutputTexture, GL_COLOR_ATTACHMENT1, true);
	if (!m_pFBO->Init())
		return false;

	return true;
}

void BasicPostProcessingPass::Render()
{

	Begin();

	m_pFBO->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_pSampler->Bind();
	m_pInputTexture->Bind(0);

	Game::GetInstance()->m_pQuad->Render();

}

void BasicPostProcessingPass::Begin()
{
	m_pShader->begin();
}

