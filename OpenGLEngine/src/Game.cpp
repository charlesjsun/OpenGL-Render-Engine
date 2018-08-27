#include "Game.h"

#include <time.h>

Game* Game::s_pInstance = NULL;

Game* Game::GetInstance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = new Game();
	}
	return s_pInstance;
}

void Game::DestroyInstance()
{
	if (s_pInstance == NULL)
	{
		return;
	}
	delete s_pInstance;
}

Game::Game()
{

}

bool Game::Init(int width, int height)
{
	m_pWindow = new Window("OpenGL Engine", width, height);
	m_pCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));
	m_pCamera->SetWindow(m_pWindow);
	
	m_LastTime = 0;
	m_CurrentTime = glfwGetTime();
	m_Frames = 0;

	//m_pFrameBuffer = new FrameBuffers();
	//m_pFrameBuffer->Init(width, height);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	m_pShaderManager = new glShaderManager();

	m_pDepthTexture = new Texture2D(TEXTURE_UNIT_DEPTH, TEX_DEPTH_STENCIL, width, height, NULL);
	m_pNormalTexture = new Texture2D(TEXTURE_UNIT_NORMAL, TEX_RGB16F, width, height, NULL);
	m_pColorTexture = new Texture2D(TEXTURE_UNIT_COLOR, TEX_RGBA, width, height, NULL);
	m_pHDRTexture = new Texture2D(TEXTURE_UNIT_HDR, TEX_RGB16F, width, height, NULL);
	m_pBloomTexture = new Texture2D(TEXTURE_UNIT_BLOOM, TEX_RGB16F, width, height, NULL);
	m_pSSAOTexture = new Texture2D(TEXTURE_UNIT_SSAO, TEX_RED, width, height, NULL);
	m_pDepthTexture->Init();
	m_pNormalTexture->Init();
	m_pColorTexture->Init();
	m_pHDRTexture->Init();
	m_pBloomTexture->Init();
	m_pSSAOTexture->Init();

	InitLights();
	InitLightVolume();

	m_pDebugSphere = MeshFactory::LoadFromOBJ("media/sphere.obj");
	m_pQuad = MeshFactory::LoadFromOBJ("media/quad.obj");


	/* RENDERING PASSES*/

	m_pGeometryPass = new GeometryPass(m_pDepthTexture, m_pNormalTexture, m_pColorTexture);
	if (!m_pGeometryPass->Init(m_pShaderManager))
		return false;

	m_pPointLightPass = new PointLightPass(m_pDepthTexture, m_pNormalTexture, m_pColorTexture, m_pHDRTexture);
	if (!m_pPointLightPass->Init(m_pShaderManager))
		return false;
	m_pPointLightPass->BeginPointLight();
	m_pPointLightPass->SetScreenSize(width, height);

	m_pDirectionalLightPass = new DirectionalLightPass(m_pDepthTexture, m_pNormalTexture, m_pColorTexture, m_pHDRTexture, m_pSSAOTexture);
	if (!m_pDirectionalLightPass->Init(m_pShaderManager))
		return false;

	m_pBloomPass = new BloomPass(m_pHDRTexture, m_pBloomTexture);
	if (!m_pBloomPass->Init(m_pShaderManager, width, height))
		return false;

	m_pHDRPass = new HDRPass(m_pHDRTexture, m_pBloomTexture);
	if (!m_pHDRPass->Init(m_pShaderManager))
		return false;
	m_pHDRPass->Begin();
	m_pHDRPass->SetExposure(1.0f);

	m_pSSAOPass = new SSAOPass(m_pDepthTexture, m_pNormalTexture, m_pSSAOTexture);
	if (!m_pSSAOPass->Init(m_pShaderManager, width, height))
		return false;
	m_pSSAOPass->Begin();
	m_pSSAOPass->SetSampleRadius(1.5f);
	m_pSSAOPass->SetNoiseScale(width, height);

	/* END RENDERING PASSES*/

	m_pMaterialManager = new MaterialManager();
	m_pMaterialManager->Init();

	char *pBlocks = new char[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{

			for (int x = 0; x < CHUNK_SIZE; x++)
			{
				for (int z = 0; z < CHUNK_SIZE; z++)
				{
					float height = CHUNK_SIZE / 4 * (((sinf((float)(i * CHUNK_SIZE + x) / 16) + sinf((float)(j * CHUNK_SIZE + z) / 16))) + 2);
					//float height = max(rand() % CHUNK_SIZE, 1);
					
					for (int y = 0; y < CHUNK_SIZE; y++)
					{
						if (y < height)
							pBlocks[(x << X_SHIFT) + (y << Y_SHIFT) + z] = (char)1;
						else
							pBlocks[(x << X_SHIFT) + (y << Y_SHIFT) + z] = (char)0;
					}
				}
			}

			pBlocks[(0 << X_SHIFT) + (31 << Y_SHIFT) + 0] = (char)1;
			pBlocks[(1 << X_SHIFT) + (31 << Y_SHIFT) + 0] = (char)1;
			pBlocks[(1 << X_SHIFT) + (31 << Y_SHIFT) + 1] = (char)1;
			pBlocks[(0 << X_SHIFT) + (31 << Y_SHIFT) + 1] = (char)1;
			pBlocks[(0 << X_SHIFT) + (30 << Y_SHIFT) + 0] = (char)1;
			pBlocks[(1 << X_SHIFT) + (30 << Y_SHIFT) + 0] = (char)1;
			pBlocks[(0 << X_SHIFT) + (30 << Y_SHIFT) + 1] = (char)1;

			ChunkCoord coord;
			coord.x = i;
			coord.y = 0;
			coord.z = j;

			m_ChunkMeshes[coord] = GreedyMesher::GenerateMesh(pBlocks);

			memset(pBlocks, 0, sizeof(pBlocks));

		}
	}

	delete[] pBlocks;

	InitCube();


	// DEBUG CUBE
#if 1
	for (int i = 0; i < 1000; i++)
	{
		objectPositions.push_back(glm::vec3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50) * glm::vec3(2));
	}
#endif

	std::cout << "GAME CONSTRUCTED" << std::endl;
	return true;
}

Game::~Game()
{
	delete m_pDebugCube;
	delete m_pLightVolume;
	delete m_pQuad;
	delete m_pDebugSphere;
	objectPositions.clear();

	for (int i = 0; i < m_PointLights.size(); i++)
		delete m_PointLights[i];

	for (auto const& entry : m_ChunkMeshes)
		delete entry.second;

	m_ChunkMeshes.clear();

	m_PointLights.clear();

	delete m_pDepthTexture;
	delete m_pNormalTexture;
	delete m_pColorTexture;
	delete m_pHDRTexture;
	delete m_pBloomTexture;
	delete m_pSSAOTexture;

	delete m_pGeometryPass;
	delete m_pPointLightPass;
	delete m_pDirectionalLightPass;
	delete m_pHDRPass;
	delete m_pBloomPass;
	delete m_pSSAOPass;

	delete m_pMaterialManager;

	delete m_pWindow;
	delete m_pCamera;
	delete m_pShaderManager;
	//delete m_pFrameBuffer;
}

void Game::InitLights()
{
	m_dirLight.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	m_dirLight.Ambient = 0.2;
	m_dirLight.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	//m_dirLight.Diffuse = glm::vec3(0.1f, 0.1f, 0.1f);
	//m_dirLight.Diffuse = glm::vec3(5.0f, 5.0f, 5.0f);

	srand(0);

	m_PointLights.push_back(new PointLight(
		glm::vec3(rand() % 256, rand() % 8 + 32, rand() % 256),
		glm::vec3(1.0f, 1.0f, 1.0f),
		20.0f
		));

	m_PointLights.push_back(new PointLight(
		glm::vec3(33, 33, 33),
		glm::vec3(1.0f, 1.0f, 1.0f),
		5.0f
		));

	for (int i = 0; i < 14; i++)
	{
		m_PointLights.push_back(new PointLight(
			glm::vec3(rand() % 256, rand() % 8 + 32, rand() % 256),
			glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX),
			50.0f
			));
	}

}

void Game::InitLightVolume()
{

	GLfloat vertices[] = {
		// front
		-1.0, -1.0,  1.0, -1.0, -1.0,  1.0,
		1.0, -1.0,  1.0, 1.0, -1.0,  1.0,
		1.0,  1.0,  1.0, 1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0, -1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0, -1.0, -1.0, -1.0,
		1.0, -1.0, -1.0, 1.0, -1.0, -1.0,
		1.0,  1.0, -1.0, 1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0, -1.0,  1.0, -1.0,
	};

	GLuint indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3,
	};
	
	std::vector<VertexAttribute> attributes;
	attributes.push_back(VertexAttribute(VERTEX_POSITION_ATTRIB, 3, GL_FLOAT, GL_FALSE, 6, 0));
	attributes.push_back(VertexAttribute(VERTEX_NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE, 6, 3));
	
	m_pLightVolume = new Mesh(std::vector<GLfloat>(vertices, vertices + 48), std::vector<GLuint>(indices, indices + 36), attributes);

}

void Game::InitCube()
{

	GLfloat vertices[] = {
																	// Back face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.8f, 1, // Bottom-left	
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.8f, 1, // top-right	
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.8f, 1, // bottom-right       
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.8f, 1,// top-left		
																//1Front face
		-0.5f, -0.5f, 0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 0.8f, 1, // bottom-left	
		 0.5f, -0.5f, 0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 0.8f, 1,  // bottom-right
		 0.5f,  0.5f, 0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 0.8f, 1,  // top-right	
		-0.5f,  0.5f, 0.5f, 0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 0.8f, 1,  // top-left	
																//1Left face
		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.8f, 1, // top-right	
		-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.8f, 1, // top-left		
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.8f, 1,  // bottom-left	
		-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.8f, 1,  // bottom-right
																//1Right face
		0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 0.8f, 1, // top-left		
		0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 0.8f, 1, // bottom-right	
		0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 0.8f, 1, // top-right	
		0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 0.8f, 1, // bottom-left	
																//1Bottom face
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.8f, 1, // top-right	
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.8f, 1, // top-left		
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.8f, 1,// bottom-left	
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.8f, 1, // bottom-right	
																//1Top face
		-0.5f, 0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 0.8f, 1,// top-left		
		 0.5f, 0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 0.8f, 1, // bottom-right	
		 0.5f, 0.5f, -0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 0.8f, 1, // top-right	
		-0.5f, 0.5f,  0.5f, 0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 0.8f, 1,// bottom-left      
	};

	GLuint indices[] = {

		0, 1, 2, 1, 0,3,	// Back face
		4, 5, 6, 6, 7,4,	// Front face
		8, 9,10,10,11,8,	// Left face
		12,13,14,13,12,15,	// Right face
		16,17,18,18,19,16,	// Bottom face
		20,21,22,21,20,23,	// Top face

	};

	std::vector<VertexAttribute> attributes;
	attributes.push_back(VertexAttribute(VERTEX_POSITION_ATTRIB, 3, GL_FLOAT, GL_FALSE, 10, 0));
	attributes.push_back(VertexAttribute(VERTEX_NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE, 10, 3));
	attributes.push_back(VertexAttribute(VERTEX_COLOR_ATTRIB, 4, GL_FLOAT, GL_FALSE, 10, 6));
	
	m_pDebugCube = new Mesh(std::vector<GLfloat>(vertices, vertices + 240), std::vector<GLuint>(indices, indices + 36), attributes);

}

bool Game::ShouldClose()
{
	return m_pWindow->IsClosed();
}