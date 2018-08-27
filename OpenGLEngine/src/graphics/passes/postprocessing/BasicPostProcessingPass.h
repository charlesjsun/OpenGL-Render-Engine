#pragma once

#include "../../../glsl/glsl.h"

#include "../../textures/Texture2D.h"
#include "../../textures/Sampler.h"

#include "../../buffers/FrameBuffer.h"



#define POSTPROC_BOX_BLUR_VS "src/graphics/shaders/postprocess/box_blur.vs.glsl"
#define POSTPROC_BOX_BLUR_FS "src/graphics/shaders/postprocess/box_blur.fs.glsl"

#define POSTPROC_BRIGHT_COLOR_EXTRACTION_VS "src/graphics/shaders/postprocess/bright_color_extraction.vs.glsl"
#define POSTPROC_BRIGHT_COLOR_EXTRACTION_FS "src/graphics/shaders/postprocess/bright_color_extraction.fs.glsl"


class BasicPostProcessingPass
{
private:
	glShader *m_pShader;

	Sampler *m_pSampler;
	Texture2D *m_pInputTexture;
	Texture2D *m_pOutputTexture;
	FrameBuffer *m_pFBO;

public:
	BasicPostProcessingPass(Texture2D *pInputTexture, Texture2D *pOutputTexture);
	~BasicPostProcessingPass();

	bool Init(glShaderManager *pShaderManager, char *vs, char *fs);

	void Render();

	void Begin();

};