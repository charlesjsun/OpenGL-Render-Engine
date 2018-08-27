#include "GeometryPass.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../Game.h"

GeometryPass::GeometryPass(Texture2D *pDepthTexture, Texture2D *pNormalTexture, Texture2D *pColorTexture)
	: m_pDepthTexture(pDepthTexture), m_pNormalTexture(pNormalTexture), m_pColorTexture(pColorTexture)
{
}

GeometryPass::~GeometryPass()
{
	delete m_pGeometryFBO;
}

bool GeometryPass::Init(glShaderManager *pShaderManager)
{
	m_pShaderGeometryPass = pShaderManager->loadfromFile("src/graphics/shaders/geometry_pass.vs.glsl", "src/graphics/shaders/geometry_pass.fs.glsl");
	if (m_pShaderGeometryPass == NULL)
		return false;

	m_MVPLocation = m_pShaderGeometryPass->GetUniformLocation("MVP");
	m_NormalMatrixLocation = m_pShaderGeometryPass->GetUniformLocation("Normal");

	if (m_MVPLocation == -1 ||
		m_NormalMatrixLocation == -1)
		return false;

	m_pGeometryFBO = new FrameBuffer();
	m_pGeometryFBO->AttachTexture(m_pDepthTexture, GL_DEPTH_STENCIL_ATTACHMENT, false);
	m_pGeometryFBO->AttachTexture(m_pNormalTexture, GL_COLOR_ATTACHMENT0, true);
	m_pGeometryFBO->AttachTexture(m_pColorTexture, GL_COLOR_ATTACHMENT1, true);

	if (!m_pGeometryFBO->Init())
		return false;

	return true;
}

void GeometryPass::Render()
{

	Begin();

	m_pGeometryFBO->Bind();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	const glm::mat4& projection = Game::GetInstance()->m_pCamera->GetProjectionMatrix();
	const glm::mat4& view = Game::GetInstance()->m_pCamera->GetViewMatrix();

#if 0

	std::map<ChunkCoord, Mesh*> meshes = Game::GetInstance()->m_ChunkMeshes;
	for (auto const& entry : meshes)
	{
		const ChunkCoord& coord = entry.first;
		const Mesh *pMesh = entry.second;

		if (pMesh == nullptr)
			continue;

		glm::mat4 model = glm::translate(glm::vec3(coord.x * CHUNK_SIZE, coord.y * CHUNK_SIZE, coord.z * CHUNK_SIZE));
		glm::mat4 mvp = projection * view * model;

		SetMVPMatrix(mvp);
		CreateAndSetNormalMatrix(model, view);

		pMesh->Render();

	}

	std::vector<PointLight*> lights = Game::GetInstance()->m_PointLights;
	for (int i = 0; i < lights.size(); i++)
	{
		glm::mat4 model = glm::translate(lights[i]->Position);
		glm::mat4 mvp = projection * view * model;

		SetMVPMatrix(mvp);
		CreateAndSetNormalMatrix(model, view);

		Game::GetInstance()->m_pDebugCube->Render();
	}
#endif

#if 1
	
	for (GLuint i = 0; i < Game::GetInstance()->objectPositions.size(); i++)
	{
		glm::mat4 model = glm::translate(glm::mat4(), Game::GetInstance()->objectPositions[i]);
		model = glm::rotate(model, Game::GetInstance()->objectPositions[i].x, glm::normalize(Game::GetInstance()->objectPositions[i]));
		glm::mat4 mvp = projection * view * model;
		
		SetMVPMatrix(mvp);
		CreateAndSetNormalMatrix(model, view);
		
		Game::GetInstance()->m_pDebugCube->Render();
	}
#endif

#if 0
	//m_pShaderGeometryPass->setUniform1f(NULL, 10000000.0f, m_DiffuseLocation);
	//glDisable(GL_CULL_FACE);

	for (GLuint i = 0; i < m_PointLights.size(); i++)
	{
		float BSphereScale = CalcPointLightBSphere(*m_PointLights[i]);

		glm::mat4 modelmat;
		modelmat = glm::translate(modelmat, m_PointLights[i]->Position);
		modelmat = glm::scale(modelmat, glm::vec3(BSphereScale, BSphereScale, BSphereScale));

		glm::mat4 mvp = projection * (view * modelmat);

		m_pShaderGeometryPass->setUniformMatrix4fv(NULL, 1, GL_FALSE, glm::value_ptr(modelmat), m_ModelMatrixLocation);

		m_pDebugCube->Render(GL_LINE_STRIP);
	}
#endif

	glDepthMask(GL_FALSE);

}

void GeometryPass::Begin() const
{
	m_pShaderGeometryPass->begin();
}

void GeometryPass::SetMVPMatrix(const glm::mat4& mvp) const
{
	glUniformMatrix4fv(m_MVPLocation, 1, GL_FALSE, glm::value_ptr(mvp));
}

void GeometryPass::CreateAndSetNormalMatrix(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix) const
{
	glm::mat3 normalMatrix = glm::mat3(viewMatrix) * glm::mat3(modelMatrix);
	glUniformMatrix3fv(m_NormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

void GeometryPass::SetNormalMatrix(const glm::mat3& normalMatrix) const
{
	glUniformMatrix3fv(m_NormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}