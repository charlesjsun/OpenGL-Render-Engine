#pragma once

#include <glm/glm.hpp>

#include "../../glsl/glsl.h"
#include "../Light.h"
#include "../Material.h"

#include "../textures/Texture2D.h"
#include "../textures/Sampler.h"

#include "../buffers/FrameBuffer.h"

class PointLightPass
{
private:
	// Stencil
	glShader *m_pShaderStencilPass;
	FrameBuffer *m_pStencilFBO;
	GLint m_StencilMVPLocation;

	// Pointlight
	glShader *m_pShaderPointLightPass;
	Texture2D *m_pDepthTexture;
	Texture2D *m_pNormalTexture;
	Texture2D *m_pColorTexture;
	Texture2D *m_pHDRTexture;
	Sampler *m_pSampler;
	FrameBuffer *m_pPointLightFBO;

	GLint m_PointLightMVPLocation;
	GLint m_MaterialsTextureLocation;
	GLint m_ScreenSizeLocation;
	GLint m_InvertProjMatrixLocation;

	struct {
		GLuint Position;
		GLuint Diffuse;
		GLuint AttenFactor;
	} m_PointLightLocation;

public:
	PointLightPass(Texture2D *pDepthTexture, Texture2D *pNormalTexture, Texture2D *pColorTexture, Texture2D *pHDRTexture);
	~PointLightPass();

	bool Init(glShaderManager *pShaderManager);

	void Render();

	void BeginStencil();
	void BeginPointLight();

	// Stencil
	void SetStencilMVP(const glm::mat4& mvp) const;

	// Pointlight
	void SetPointLightMVP(const glm::mat4& mvp) const;
	void SetScreenSize(float width, float height) const;
	void SetInvertProjMatrix(const glm::mat4& invertProjMatrix) const;
	void SetPointLight(const PointLight& pointLight, const glm::mat4& view) const;

};

