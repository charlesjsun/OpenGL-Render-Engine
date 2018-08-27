#pragma once

#include <GL/glew.h>
#include <iostream>

#define FBO_DEPTH_TEXTURE_UNIT		0
#define FBO_DEPTH_ATTACHMENT		GL_DEPTH_STENCIL_ATTACHMENT
#define FBO_DEPTH_TEXTURE			GL_TEXTURE0 + FBO_DEPTH_TEXTURE_UNIT

#define FBO_NORMAL_TEXTURE_UNIT		1
#define FBO_NORMAL_ATTACHMENT		GL_COLOR_ATTACHMENT0 + FBO_NORMAL_TEXTURE_UNIT
#define FBO_NORMAL_TEXTURE			GL_TEXTURE0 + FBO_NORMAL_TEXTURE_UNIT

#define FBO_COLOR_TEXTURE_UNIT		2
#define FBO_COLOR_ATTACHMENT		GL_COLOR_ATTACHMENT0 + FBO_COLOR_TEXTURE_UNIT
#define FBO_COLOR_TEXTURE			GL_TEXTURE0 + FBO_COLOR_TEXTURE_UNIT

#define FBO_HDR_TEXTURE_UNIT		3
#define FBO_HDR_ATTACHMENT			GL_COLOR_ATTACHMENT0 + FBO_HDR_TEXTURE_UNIT
#define FBO_HDR_TEXTURE				GL_TEXTURE0 + FBO_HDR_TEXTURE_UNIT

#define FBO_SSAO_TEXTURE_UNIT		4
#define FBO_SSAO_ATTACHMENT			GL_COLOR_ATTACHMENT0 + FBO_SSAO_TEXTURE_UNIT
#define FBO_SSAO_TEXTURE			GL_TEXTURE0 + FBO_SSAO_TEXTURE_UNIT

#define FBO_SSAO_BLUR_TEXTURE_UNIT	5
#define FBO_SSAO_BLUR_ATTACHMENT	GL_COLOR_ATTACHMENT0 + FBO_SSAO_BLUR_TEXTURE_UNIT
#define FBO_SSAO_BLUR_TEXTURE		GL_TEXTURE0	+ FBO_SSAO_BLUR_TEXTURE_UNIT

class FrameBuffers
{
private:
	GLuint m_FBO; // Framebuffer IDs

	//GLuint m_PositionTexture;
	GLuint m_ColorTexture;
	GLuint m_NormalTexture;
	GLuint m_DepthTexture;

	GLuint m_HDRTexture;

	GLuint m_SSAOTexture;
	GLuint m_SSAOBlurTexture;

public:
	FrameBuffers();

	~FrameBuffers();

	bool Init(const GLuint& WindowWidth, const GLuint& WindowHeight);

	void StartFrame();
	void BindForGeomPass();
	void BindForStencilPass();
	void BindForLightPass();
	void BindForSSAOPass();
	void BindForSSAOBlurPass();
	void BindForHDRPass();

};