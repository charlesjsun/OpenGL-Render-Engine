#include "BloomPass.h"

BloomPass::BloomPass(Texture2D *pHDRTexture, Texture2D *pBloomTexture)
	: m_pHDRTexture(pHDRTexture), m_pBloomTexture(pBloomTexture)
{

}

BloomPass::~BloomPass()
{
	delete m_pExtractedColorTexture;
	delete m_pBrightColorExtraction;
	delete m_pBlurPass;
}

bool BloomPass::Init(glShaderManager *pShaderManager, int width, int height)
{

	m_pExtractedColorTexture = new Texture2D(0, TEX_RGB16F, width, height, NULL);
	m_pExtractedColorTexture->Init();

	m_pBrightColorExtraction = new BasicPostProcessingPass(m_pHDRTexture, m_pExtractedColorTexture);
	if (!m_pBrightColorExtraction->Init(pShaderManager, POSTPROC_BRIGHT_COLOR_EXTRACTION_VS, POSTPROC_BRIGHT_COLOR_EXTRACTION_FS))
		return false;

	m_pBlurPass = new BasicPostProcessingPass(m_pExtractedColorTexture, m_pBloomTexture);
	if (!m_pBlurPass->Init(pShaderManager, POSTPROC_BOX_BLUR_VS, POSTPROC_BOX_BLUR_FS))
		return false;

}

void BloomPass::Render()
{

	m_pBrightColorExtraction->Render();
	m_pBlurPass->Render();

}