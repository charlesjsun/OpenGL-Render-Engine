#pragma once

#include "Mesh.h"
#include "VoxelFace.h"

#include "../../world/chunks/Chunk.h"

#define BAKED_AO 0

class GreedyMesher
{

#define VERTEX_SIZE 10

public:
	static Mesh* GenerateMesh(const char *pBlocks);

private:
	static void WriteQuad(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices,
		glm::vec3& bottomLeft, glm::vec3& topLeft, glm::vec3& topRight, glm::vec3& bottomRight, glm::vec4& color, VoxelFace::Face face, bool backFace
#if BAKED_AO
		, char ao
#endif
		);

};