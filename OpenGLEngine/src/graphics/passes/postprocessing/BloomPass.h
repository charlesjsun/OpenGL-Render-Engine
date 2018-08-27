#pragma once

#include "../../../glsl/glsl.h"

#include "../../textures/Texture2D.h"
#include "../../textures/Sampler.h"

#include "../../buffers/FrameBuffer.h"

#include "BasicPostProcessingPass.h"

class BloomPass
{
private:
	Texture2D *m_pHDRTexture;
	Texture2D *m_pBloomTexture;

	Texture2D *m_pExtractedColorTexture;

	BasicPostProcessingPass *m_pBrightColorExtraction;
	BasicPostProcessingPass *m_pBlurPass;

public:
	BloomPass(Texture2D *pHDRTexture, Texture2D *pBloomTexture);
	~BloomPass();

	bool Init(glShaderManager *pShaderManager, int width, int height);

	void Render();

};

