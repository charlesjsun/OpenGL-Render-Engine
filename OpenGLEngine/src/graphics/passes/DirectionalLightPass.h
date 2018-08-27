#pragma once

#include <glm/glm.hpp>

#include "../../glsl/glsl.h"
#include "../Light.h"
#include "../Material.h"

#include "../textures/Texture2D.h"
#include "../textures/Sampler.h"

#include "../buffers/FrameBuffer.h"


class DirectionalLightPass
{
private:
	glShader *m_pShaderDirectionalLightPass;

	Texture2D *m_pDepthTexture;
	Texture2D *m_pColorTexture;
	Texture2D *m_pNormalTexture;
	Texture2D *m_pHDRTexture;
	Texture2D *m_pSSAOTexture;
	Sampler *m_pSampler;
	FrameBuffer *m_pDirectionalLightFBO;

	struct {
		GLuint Direction;
		GLuint Ambient;
		GLuint Diffuse;
	} m_DirectionalLightLocation;

public:
	DirectionalLightPass(Texture2D *pDepthTexture, Texture2D *pNormalTexture, Texture2D *pColorTexture, Texture2D *pHDRTexture, Texture2D *pSSAOTexture);
	~DirectionalLightPass();

	bool Init(glShaderManager *pShaderManager);

	void Render();

	void Begin();

	void SetDirectionalLight(const DirectionalLight& directionalLight, const glm::mat4& view) const;

};