#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>

#include "application/Window.h"

#include "glsl/glsl.h"

//#include "graphics/buffers/FrameBuffers.h"

#include "graphics/Camera.h"
#include "graphics/Light.h"
#include "graphics/Material.h"

#include "graphics/mesh/Mesh.h"
#include "graphics/mesh/MeshFactory.h"
#include "graphics/mesh/GreedyMesher.h"

#include "graphics/passes/GeometryPass.h"
#include "graphics/passes/PointLightPass.h"
#include "graphics/passes/DirectionalLightPass.h"
#include "graphics/passes/HDRPass.h"

#include "graphics/passes/postprocessing/BasicPostProcessingPass.h"
#include "graphics/passes/postprocessing/SSAOPass.h"
#include "graphics/passes/postprocessing/BloomPass.h"

#include "world/chunks/Chunk.h"

#include "utils/Timer.h"


//class Renderer;
//class ChunkManager;

#define SAFE_DELETE(x) if (x != NULL) delete x;

class Game
{

	// DEBUGING PURPOSE REMOVE LATER
	friend class GeometryPass;
	friend class SSAOPass;
	friend class BasicPostProcessingPass;
	friend class PointLightPass;
	friend class DirectionalLightPass;
	friend class HDRPass;

private:
	static Game *s_pInstance;

private:
	Window *m_pWindow;
	//Renderer *m_pRenderer;
	//ChunkManager *m_pChunkManager;
	Camera *m_pCamera;

	glShaderManager *m_pShaderManager;

	Texture2D *m_pDepthTexture;
	Texture2D *m_pNormalTexture;
	Texture2D *m_pColorTexture;
	Texture2D *m_pHDRTexture;
	Texture2D *m_pBloomTexture;
	Texture2D *m_pSSAOTexture;
	
	GeometryPass *m_pGeometryPass;
	PointLightPass *m_pPointLightPass;
	DirectionalLightPass *m_pDirectionalLightPass;
	BloomPass *m_pBloomPass;
	HDRPass *m_pHDRPass;
	SSAOPass *m_pSSAOPass;

	MaterialManager *m_pMaterialManager;

	Mesh *m_pLightVolume;
	Mesh *m_pQuad;
	
	Mesh *m_pDebugCube;
	Mesh *m_pDebugSphere;

	std::map<ChunkCoord, Mesh*> m_ChunkMeshes;

	// DEBUG LIGHTING
	DirectionalLight m_dirLight;
	std::vector<PointLight*> m_PointLights;

	std::vector<glm::vec3> objectPositions;

	// FPS Counter
	GLdouble m_LastTime;
	GLdouble m_CurrentTime;
	GLuint m_Frames;

	//FrameBuffers *m_pFrameBuffer;

public:
	static Game* GetInstance();
	static void DestroyInstance();

public:

	bool Init(int width, int height);

	void Update();
	void Render();

	bool ShouldClose();

private:
	Game();
	~Game();

	// DEBUG
	void InitCube();
	void InitLightVolume();
	void InitLights();

};