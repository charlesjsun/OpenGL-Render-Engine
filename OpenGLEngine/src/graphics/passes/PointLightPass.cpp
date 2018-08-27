#include "PointLightPass.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../buffers/FrameBuffers.h"

#include "../../Game.h"

PointLightPass::PointLightPass(Texture2D *pDepthTexture, Texture2D *pNormalTexture, Texture2D *pColorTexture, Texture2D *pHDRTexture)
	: m_pDepthTexture(pDepthTexture), m_pNormalTexture(pNormalTexture), m_pColorTexture(pColorTexture), m_pHDRTexture(pHDRTexture)
{

}

PointLightPass::~PointLightPass()
{
	delete m_pStencilFBO;
	delete m_pPointLightFBO;
	delete m_pSampler;
}

bool PointLightPass::Init(glShaderManager *pShaderManager)
{

	m_pShaderStencilPass = pShaderManager->loadfromFile("src/graphics/shaders/stencil_pass.vs.glsl", "src/graphics/shaders/stencil_pass.fs.glsl");
	if (m_pShaderStencilPass == NULL)
		return false;

	m_pShaderPointLightPass = pShaderManager->loadfromFile("src/graphics/shaders/point_light_pass.vs.glsl", "src/graphics/shaders/point_light_pass.fs.glsl");
	if (m_pShaderPointLightPass == NULL)
		return false;

	m_StencilMVPLocation = m_pShaderStencilPass->GetUniformLocation("MVP");
	
	m_PointLightMVPLocation		= m_pShaderPointLightPass->GetUniformLocation("MVP");
	m_ScreenSizeLocation		= m_pShaderPointLightPass->GetUniformLocation("ScreenSize");
	m_InvertProjMatrixLocation	= m_pShaderPointLightPass->GetUniformLocation("InvertProj");

	m_PointLightLocation.Position		= m_pShaderPointLightPass->GetUniformLocation("uPointLight.Position");
	m_PointLightLocation.Diffuse		= m_pShaderPointLightPass->GetUniformLocation("uPointLight.Diffuse");
	m_PointLightLocation.AttenFactor	= m_pShaderPointLightPass->GetUniformLocation("uPointLight.AttenFactor");

	if (m_StencilMVPLocation == -1 ||
		m_PointLightMVPLocation == -1 ||
		m_ScreenSizeLocation == -1 ||
		m_InvertProjMatrixLocation == -1 ||
		m_PointLightLocation.Position == -1 ||
		m_PointLightLocation.Diffuse == -1 ||
		m_PointLightLocation.AttenFactor == -1
		)
		return false;

	m_pStencilFBO = new FrameBuffer();
	m_pStencilFBO->AttachTexture(m_pDepthTexture, GL_DEPTH_STENCIL_ATTACHMENT, false);
	if (!m_pStencilFBO->Init())
		return false;

	const GLuint samplerUnits[] = {
		m_pDepthTexture->GetTextureUnit(),
		m_pNormalTexture->GetTextureUnit(),
		m_pColorTexture->GetTextureUnit(),
		TEXTURE_UNIT_MATERIALS
	};
	m_pSampler = new Sampler(4, samplerUnits);
	m_pSampler->AddParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	m_pSampler->AddParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	m_pSampler->AddParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_pSampler->AddParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_pSampler->Init();

	m_pPointLightFBO = new FrameBuffer();
	m_pPointLightFBO->AttachTexture(m_pDepthTexture, GL_DEPTH_STENCIL_ATTACHMENT, false);
	m_pPointLightFBO->AttachTexture(m_pNormalTexture, GL_COLOR_ATTACHMENT0, false);
	m_pPointLightFBO->AttachTexture(m_pColorTexture, GL_COLOR_ATTACHMENT1, false);
	m_pPointLightFBO->AttachTexture(m_pHDRTexture, GL_COLOR_ATTACHMENT2, true);
	if (!m_pPointLightFBO->Init())
		return false;

	return true;
}

void PointLightPass::Render()
{

	glEnable(GL_STENCIL_TEST);

	std::vector<PointLight*> lights = Game::GetInstance()->m_PointLights;
	
	const glm::mat4& projection = Game::GetInstance()->m_pCamera->GetProjectionMatrix();
	const glm::mat4& view = Game::GetInstance()->m_pCamera->GetViewMatrix();
	const glm::mat4& inverseProjection = Game::GetInstance()->m_pCamera->GetInverseProjectionMatrix();

	for (GLuint i = 0; i < lights.size(); i++)
	{
		// STENCIL PASS
		BeginStencil();
		m_pStencilFBO->Bind();

		glEnable(GL_DEPTH_TEST);

		glDisable(GL_CULL_FACE);

		glClear(GL_STENCIL_BUFFER_BIT);

		// We need the stencil test to be enabled but we want it
		// to succeed always. Only the depth test matters.
		glStencilFunc(GL_ALWAYS, 0, 0);

		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		float lightVolumeScale = lights[i]->GetRadius();
		//float lightVolumeScale = CalcPointLightBSphere(*m_PointLights[i]);

		glm::mat4 model;
		model = glm::translate(model, lights[i]->Position);
		model = glm::scale(model, glm::vec3(lightVolumeScale, lightVolumeScale, lightVolumeScale));


		glm::mat4 mvp = projection * view * model;

		SetStencilMVP(mvp);

		Game::GetInstance()->m_pLightVolume->Render();


		// POINT LIGHT PASS

		BeginPointLight();

		m_pPointLightFBO->Bind();

		if (i == 0)
			glClear(GL_COLOR_BUFFER_BIT);

		m_pSampler->Bind();
		m_pDepthTexture->Bind();
		m_pNormalTexture->Bind();
		m_pColorTexture->Bind();
		Game::GetInstance()->m_pMaterialManager->BindMaterialsTexture();

		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		glDisable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		SetPointLight(*lights[i], view);

		SetPointLightMVP(mvp);

		SetInvertProjMatrix(inverseProjection);

		Game::GetInstance()->m_pLightVolume->Render();

	}

	glDisable(GL_STENCIL_TEST);

}

void PointLightPass::BeginStencil()
{
	m_pShaderStencilPass->begin();
}

void PointLightPass::BeginPointLight()
{
	m_pShaderPointLightPass->begin();
}

void PointLightPass::SetStencilMVP(const glm::mat4& mvp) const
{
	glUniformMatrix4fv(m_StencilMVPLocation, 1, GL_FALSE, glm::value_ptr(mvp));
}

void PointLightPass::SetPointLightMVP(const glm::mat4& mvp) const
{
	glUniformMatrix4fv(m_PointLightMVPLocation, 1, GL_FALSE, glm::value_ptr(mvp));
}

void PointLightPass::SetScreenSize(float width, float height) const
{
	glUniform2f(m_ScreenSizeLocation, width, height);
}

void PointLightPass::SetInvertProjMatrix(const glm::mat4& invertProjMatrix) const
{
	glUniform4f(m_InvertProjMatrixLocation, invertProjMatrix[0][0], invertProjMatrix[1][1], invertProjMatrix[2][3], invertProjMatrix[3][3]);
}

void PointLightPass::SetPointLight(const PointLight& pointLight, const glm::mat4& view) const
{
	//glm::vec4 position = view * glm::vec4(pPointLight->Position, 1.0);
	glm::vec3 position(
		view[0][0] * pointLight.Position.x + view[1][0] * pointLight.Position.y + view[2][0] * pointLight.Position.z + view[3][0],
		view[0][1] * pointLight.Position.x + view[1][1] * pointLight.Position.y + view[2][1] * pointLight.Position.z + view[3][1],
		view[0][2] * pointLight.Position.x + view[1][2] * pointLight.Position.y + view[2][2] * pointLight.Position.z + view[3][2]
		);
	glUniform3f(m_PointLightLocation.Position, position.x, position.y, position.z);
	glUniform3f(m_PointLightLocation.Diffuse, pointLight.Diffuse.x, pointLight.Diffuse.y, pointLight.Diffuse.z);
	glUniform1f(m_PointLightLocation.AttenFactor, pointLight.AttenFactor);
}