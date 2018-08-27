#include "DirectionalLightPass.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Game.h"

DirectionalLightPass::DirectionalLightPass(Texture2D *pDepthTexture, Texture2D *pNormalTexture, Texture2D *pColorTexture, Texture2D *pHDRTexture, Texture2D *pSSAOTexture)
	: m_pDepthTexture(pDepthTexture), m_pNormalTexture(pNormalTexture), m_pColorTexture(pColorTexture), m_pHDRTexture(pHDRTexture), m_pSSAOTexture(pSSAOTexture)
{

}

DirectionalLightPass::~DirectionalLightPass()
{
	delete m_pSampler;
	delete m_pDirectionalLightFBO;
}

bool DirectionalLightPass::Init(glShaderManager *pShaderManager)
{

	m_pShaderDirectionalLightPass = pShaderManager->loadfromFile("src/graphics/shaders/directional_light_pass.vs.glsl", "src/graphics/shaders/directional_light_pass.fs.glsl");
	if (m_pShaderDirectionalLightPass == NULL)
		return false;

	m_DirectionalLightLocation.Direction = m_pShaderDirectionalLightPass->GetUniformLocation("DirLight.Direction");
	m_DirectionalLightLocation.Ambient = m_pShaderDirectionalLightPass->GetUniformLocation("DirLight.Ambient");
	m_DirectionalLightLocation.Diffuse = m_pShaderDirectionalLightPass->GetUniformLocation("DirLight.Diffuse");

	if (m_DirectionalLightLocation.Direction == -1 ||
		m_DirectionalLightLocation.Ambient == -1 ||
		m_DirectionalLightLocation.Diffuse == -1
		)
		return false;

	const GLuint samplerUnits[] = {
		m_pNormalTexture->GetTextureUnit(),
		m_pColorTexture->GetTextureUnit(),
		m_pSSAOTexture->GetTextureUnit(),
		TEXTURE_UNIT_MATERIALS
	};
	m_pSampler = new Sampler(4, samplerUnits);
	m_pSampler->AddParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	m_pSampler->AddParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	m_pSampler->AddParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	m_pSampler->AddParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_pSampler->Init();

	m_pDirectionalLightFBO = new FrameBuffer();
	m_pDirectionalLightFBO->AttachTexture(m_pNormalTexture, GL_COLOR_ATTACHMENT0, false);
	m_pDirectionalLightFBO->AttachTexture(m_pColorTexture, GL_COLOR_ATTACHMENT1, false);
	m_pDirectionalLightFBO->AttachTexture(m_pSSAOTexture, GL_COLOR_ATTACHMENT2, false);
	m_pDirectionalLightFBO->AttachTexture(m_pHDRTexture, GL_COLOR_ATTACHMENT3, true);
	if (!m_pDirectionalLightFBO->Init())
		return false;

	return true;
}

void DirectionalLightPass::Render()
{
	Begin();

	m_pDirectionalLightFBO->Bind();

	m_pSampler->Bind();
	m_pNormalTexture->Bind();
	m_pColorTexture->Bind();
	m_pSSAOTexture->Bind();
	Game::GetInstance()->m_pMaterialManager->BindMaterialsTexture();

	SetDirectionalLight(Game::GetInstance()->m_dirLight, Game::GetInstance()->m_pCamera->GetViewMatrix());

	glDisable(GL_CULL_FACE);

	Game::GetInstance()->m_pQuad->Render();

}

void DirectionalLightPass::Begin()
{
	m_pShaderDirectionalLightPass->begin();
}

void DirectionalLightPass::SetDirectionalLight(const DirectionalLight& directionalLight, const glm::mat4& view) const
{
	glm::vec3 direction = glm::mat3(view) * directionalLight.Direction;
	glUniform3f(m_DirectionalLightLocation.Direction, direction.x, direction.y, direction.z);
	glUniform1f(m_DirectionalLightLocation.Ambient, directionalLight.Ambient);
	glUniform3f(m_DirectionalLightLocation.Diffuse, directionalLight.Diffuse.x, directionalLight.Diffuse.y, directionalLight.Diffuse.z);
}
