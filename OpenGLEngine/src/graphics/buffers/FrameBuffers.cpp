#include "FrameBuffers.h"

FrameBuffers::FrameBuffers()
{
	m_FBO = -1;
	m_DepthTexture = -1;
	m_NormalTexture = -1;
	m_ColorTexture = -1;
	m_HDRTexture = -1;
	m_SSAOTexture = -1;
	m_SSAOBlurTexture = -1;
}

FrameBuffers::~FrameBuffers()
{
	if (m_FBO != -1) 
	{
		glDeleteFramebuffers(1, &m_FBO);
	}

	if (m_DepthTexture != -1) 
	{
		glDeleteTextures(1, &m_DepthTexture);
	}

	if (m_ColorTexture != -1)
	{
		glDeleteTextures(1, &m_ColorTexture);
	}

	if (m_NormalTexture != -1) 
	{
		glDeleteTextures(1, &m_NormalTexture);
	}

	if (m_HDRTexture != -1) 
	{
		glDeleteTextures(1, &m_HDRTexture);
	}

	if (m_SSAOTexture != -1)
	{
		glDeleteTextures(1, &m_SSAOTexture);
	}

	if (m_SSAOBlurTexture != -1)
	{
		glDeleteTextures(1, &m_SSAOBlurTexture);
	}

}

bool FrameBuffers::Init(const GLuint& WindowWidth, const GLuint& WindowHeight)
{
	// Create the FBO
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);

	// - Position texture
	// glGenTextures(1, &m_PositionTexture);
	// glBindTexture(GL_TEXTURE_2D, m_PositionTexture);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PositionTexture, 0);
	
	// - Depth Texture
	glGenTextures(1, &m_DepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, WindowWidth, WindowHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, FBO_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

	// - Normal texture
	glGenTextures(1, &m_NormalTexture);
	glBindTexture(GL_TEXTURE_2D, m_NormalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, FBO_NORMAL_ATTACHMENT, GL_TEXTURE_2D, m_NormalTexture, 0);
	
	// - Color texture
	glGenTextures(1, &m_ColorTexture);
	glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowWidth, WindowHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, FBO_COLOR_ATTACHMENT, GL_TEXTURE_2D, m_ColorTexture, 0);


	// - HDR Texture
	glGenTextures(1, &m_HDRTexture);
	glBindTexture(GL_TEXTURE_2D, m_HDRTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, FBO_HDR_ATTACHMENT, GL_TEXTURE_2D, m_HDRTexture, 0);

	// - SSAO Texture
	glGenTextures(1, &m_SSAOTexture);
	glBindTexture(GL_TEXTURE_2D, m_SSAOTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, WindowWidth, WindowHeight, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, FBO_SSAO_ATTACHMENT, GL_TEXTURE_2D, m_SSAOTexture, 0);

	// - SSAO Blur Texture
	glGenTextures(1, &m_SSAOBlurTexture);
	glBindTexture(GL_TEXTURE_2D, m_SSAOBlurTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, FBO_SSAO_BLUR_ATTACHMENT, GL_TEXTURE_2D, m_SSAOBlurTexture, 0);


	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) 
	{
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return true;
}

void FrameBuffers::StartFrame()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
	glDrawBuffer(FBO_HDR_ATTACHMENT);
	glClear(GL_COLOR_BUFFER_BIT);
}

void FrameBuffers::BindForGeomPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);

	GLenum DrawBuffers[] = 
	{ 
		FBO_NORMAL_ATTACHMENT, 
		FBO_COLOR_ATTACHMENT
	};

	glDrawBuffers(2, DrawBuffers);
}

void FrameBuffers::BindForStencilPass()
{
	// CHANGE TO GL_COLOR_ATTACHMENT4 FOR AWESOMENESS
	//glDrawBuffer(GL_COLOR_ATTACHMENT4);
	glDrawBuffer(GL_NONE);
}

void FrameBuffers::BindForLightPass()
{
	glDrawBuffer(FBO_HDR_ATTACHMENT);

	glActiveTexture(FBO_DEPTH_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);

	glActiveTexture(FBO_NORMAL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, m_NormalTexture);

	glActiveTexture(FBO_COLOR_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, m_ColorTexture);

	glActiveTexture(FBO_SSAO_BLUR_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, m_SSAOBlurTexture);

}

void FrameBuffers::BindForSSAOPass()
{
	glDrawBuffer(FBO_SSAO_ATTACHMENT);

	glActiveTexture(FBO_DEPTH_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);

	glActiveTexture(FBO_NORMAL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, m_NormalTexture);
}

void FrameBuffers::BindForSSAOBlurPass()
{
	glDrawBuffer(FBO_SSAO_BLUR_ATTACHMENT);

	glActiveTexture(FBO_SSAO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, m_SSAOTexture);
}

void FrameBuffers::BindForHDRPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glActiveTexture(FBO_HDR_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, m_HDRTexture);
}


