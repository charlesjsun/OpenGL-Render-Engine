#pragma once

#include <glm/glm.hpp>
#include "../../glsl/glsl.h"

#include "../textures/Texture2D.h"

#include "../buffers/FrameBuffer.h"

class GeometryPass
{
private:
	glShader *m_pShaderGeometryPass;
	GLint m_MVPLocation;
	GLint m_NormalMatrixLocation;
	GLint m_DiffuseLocation;

	Texture2D *m_pDepthTexture;
	Texture2D *m_pNormalTexture;
	Texture2D *m_pColorTexture;

	FrameBuffer *m_pGeometryFBO;

public:
	GeometryPass(Texture2D *pDepthTexture, Texture2D *pNormalTexture, Texture2D *pColorTexture);
	~GeometryPass();

	bool Init(glShaderManager *pShaderManager);

	void Render();

	void Begin() const;

	void SetMVPMatrix(const glm::mat4& mvp) const;
	void CreateAndSetNormalMatrix(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix) const;
	void SetNormalMatrix(const glm::mat3& normalMatrix) const;

};