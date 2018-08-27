#include "GreedyMesher.h"
#include "../../utils/CharUtils.h"

bool IsFaceVisible(glm::ivec3 pos, VoxelFace::Face face, const char *pBlocks)
{
	glm::ivec3 vec = pos + VoxelFace::GetOffset(face);

	if (vec.x < 0 || vec.y < 0 || vec.z < 0 || vec.x >= CHUNK_SIZE || vec.y >= CHUNK_SIZE || vec.z >= CHUNK_SIZE)
		return true;

	char type = pBlocks[(vec.x << X_SHIFT) + (vec.y << Y_SHIFT) + vec.z];
	return type == 0;
}


char VertexAO(int s1, int s2, int c)
{
	if (s1 == 1 && s2 == 1)
	{
		return 0;
	}
	return (char)(3 - (s1 + s2 + c));
}


char FaceAO(char a00, char a01, char a02,
			 char a10, /* --- */ char a12,
			 char a20, char a21, char a22)
{
	int s00 = (a00 == 0) ? 0 : 1;
	int s01 = (a01 == 0) ? 0 : 1;
	int s02 = (a02 == 0) ? 0 : 1;
	int s10 = (a10 == 0) ? 0 : 1;
	int s12 = (a12 == 0) ? 0 : 1;
	int s20 = (a20 == 0) ? 0 : 1;
	int s21 = (a21 == 0) ? 0 : 1;
	int s22 = (a22 == 0) ? 0 : 1;

	char aoValues = 0;

	CharUtils::SetCharBits(aoValues, 0, 2, VertexAO(s21, s10, s20));
	CharUtils::SetCharBits(aoValues, 2, 2, VertexAO(s12, s21, s22));
	CharUtils::SetCharBits(aoValues, 4, 2, VertexAO(s10, s01, s00));
	CharUtils::SetCharBits(aoValues, 6, 2, VertexAO(s01, s12, s02));

	return aoValues;
}

char GetBlock(const glm::ivec3& vec, const char *pBlocks)
{
	if (vec.x < 0 || vec.y < 0 || vec.z < 0 || vec.x >= CHUNK_SIZE || vec.y >= CHUNK_SIZE || vec.z >= CHUNK_SIZE)
		return 0;
	return pBlocks[(vec.x << X_SHIFT) + (vec.y << Y_SHIFT) + vec.z];
}

char GetBlock(int x, int y, int z, const char *pBlocks)
{
	if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE)
		return 0;
	return pBlocks[(x << X_SHIFT) + (y << Y_SHIFT) + z];
}

char GetAOValues(const glm::ivec3& loc, VoxelFace::Face face, const char *pBlocks)
{

	int x = loc.x;
	int y = loc.y;
	int z = loc.z;

	char aoValue;

	switch (face)
	{
	case VoxelFace::Face::TOP:
		aoValue = FaceAO(	GetBlock(x - 1, y + 1, z - 1, pBlocks), GetBlock(x, y + 1, z - 1, pBlocks), GetBlock(x + 1, y + 1, z - 1, pBlocks),
							GetBlock(x - 1, y + 1, z, pBlocks),											GetBlock(x + 1, y + 1, z, pBlocks),
							GetBlock(x - 1, y + 1, z + 1, pBlocks), GetBlock(x, y + 1, z + 1, pBlocks), GetBlock(x + 1, y + 1, z + 1, pBlocks));
		break;
	case VoxelFace::Face::BOTTOM:
		aoValue = FaceAO(	GetBlock(x - 1, y - 1, z + 1, pBlocks), GetBlock(x - 1, y - 1, z, pBlocks), GetBlock(x - 1, y - 1, z - 1, pBlocks),
							GetBlock(x, y - 1, z + 1, pBlocks),											GetBlock(x, y - 1, z - 1, pBlocks),
							GetBlock(x + 1, y - 1, z - 1, pBlocks), GetBlock(x + 1, y - 1, z, pBlocks), GetBlock(x + 1, y - 1, z + 1, pBlocks));
		break;
	case VoxelFace::Face::LEFT:
		aoValue = FaceAO(	GetBlock(x - 1, y + 1, z - 1, pBlocks), GetBlock(x - 1, y + 1, z, pBlocks), GetBlock(x - 1, y + 1, z + 1, pBlocks),
							GetBlock(x - 1, y,	   z - 1, pBlocks),										GetBlock(x - 1, y,     z + 1, pBlocks),
							GetBlock(x - 1, y - 1, z - 1, pBlocks), GetBlock(x - 1, y - 1, z, pBlocks), GetBlock(x - 1, y - 1, z + 1, pBlocks));
		break;
	case VoxelFace::Face::RIGHT:
		aoValue = FaceAO(	GetBlock(x + 1, y + 1, z + 1, pBlocks), GetBlock(x + 1, y + 1, z, pBlocks), GetBlock(x + 1, y + 1, z - 1, pBlocks),
							GetBlock(x + 1, y, z + 1, pBlocks),											GetBlock(x + 1, y, z - 1, pBlocks),
							GetBlock(x + 1, y - 1, z + 1, pBlocks), GetBlock(x + 1, y - 1, z, pBlocks), GetBlock(x + 1, y - 1, z - 1, pBlocks));
		break;
	case VoxelFace::Face::FRONT:
		aoValue = FaceAO(	GetBlock(x - 1, y + 1, z + 1, pBlocks), GetBlock(x, y + 1, z + 1, pBlocks), GetBlock(x + 1, y + 1, z + 1, pBlocks),
							GetBlock(x - 1, y, z + 1, pBlocks),											GetBlock(x + 1, y, z + 1, pBlocks),
							GetBlock(x - 1, y - 1, z + 1, pBlocks), GetBlock(x, y - 1, z + 1, pBlocks), GetBlock(x + 1, y - 1, z + 1, pBlocks));
		break;
	case VoxelFace::Face::BACK:
		aoValue = FaceAO(	GetBlock(x + 1, y + 1, z - 1, pBlocks), GetBlock(x, y + 1, z - 1, pBlocks), GetBlock(x - 1, y + 1, z - 1, pBlocks),
							GetBlock(x + 1,		y, z - 1, pBlocks),								GetBlock(x - 1,		y, z - 1, pBlocks),
							GetBlock(x + 1, y - 1, z - 1, pBlocks), GetBlock(x, y - 1, z - 1, pBlocks), GetBlock(x - 1, y - 1, z - 1, pBlocks));
		break;
	}

	return aoValue;
}

Mesh* GreedyMesher::GenerateMesh(const char *pBlocks)
{

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	glm::ivec3 tmpI;

	int i, j, k, l, h, w, u, v, n, r, s, t;

	VoxelFace::Face face;

	int x[] = { 0, 0, 0 };
	int q[] = { 0, 0, 0 };
	int du[] = { 0, 0, 0 };
	int dv[] = { 0, 0, 0 };

	char mask[CHUNK_SIZE * CHUNK_SIZE];

#if BAKED_AO
	char ao[CHUNK_SIZE * CHUNK_SIZE];
#endif

	// First pass is for front face, second pass is for back face
	for (bool backFace = true, b = false; b != backFace; backFace = backFace && b, b = !b)
	{
		// Loop over all 3 dimensions
		for (int d = 0; d < 3; d++)
		{
			u = (d + 1) % 3;
			v = (d + 2) % 3;

			x[0] = 0;
			x[1] = 0;
			x[2] = 0;
			q[0] = 0;
			q[1] = 0;
			q[2] = 0;
			q[d] = 1;

			// Keep track of what face we are computing
			if (d == 0)
			{
				face = backFace ? VoxelFace::LEFT : VoxelFace::RIGHT;
			}
			else if (d == 1)
			{
				face = backFace ? VoxelFace::BOTTOM : VoxelFace::TOP;
			}
			else if (d == 2)
			{
				face = backFace ? VoxelFace::BACK : VoxelFace::FRONT;
			}
			// Loop over the entire voxel volume to generate the mask
			for (x[d] = 0; x[d] < CHUNK_SIZE; x[d]++)
			{
				n = 0;
				for (x[v] = 0; x[v] < CHUNK_SIZE; x[v]++)
				{
					for (x[u] = 0; x[u] < CHUNK_SIZE; x[u]++)
					{
						tmpI = glm::vec3(x[0], x[1], x[2]);
						mask[n] = IsFaceVisible(tmpI, face, pBlocks) ? GetBlock(tmpI, pBlocks) : 0;
						
#if BAKED_AO
						ao[n] = GetAOValues(tmpI, face, pBlocks);
#endif
						n++;
					}
				}

				n = 0;
				for (j = 0; j < CHUNK_SIZE; j++)
				{
					for (i = 0; i < CHUNK_SIZE; )
					{
						// Loop until we find a start point
						if (mask[n] != 0)
						{
							// Find the width of this mask section, w == current width
							for (w = 1; w + i < CHUNK_SIZE && mask[n + w] != 0 && mask[n + w] == mask[n]
#if BAKED_AO
								&& ao[n + w] == ao[n]
#endif
								; w++)
							{
							}

							bool done = false;
							// find the height of the mask section, h == current height
							for (h = 1; j + h < CHUNK_SIZE; h++)
							{
								// make sure a whoel row of each height matches the width
								for (k = 0; k < w; k++)
								{
									if (mask[n + k + h * CHUNK_SIZE] == 0 || mask[n + k + h * CHUNK_SIZE] != mask[n]
#if BAKED_AO
										|| ao[n + k + h * CHUNK_SIZE] != ao[n]
#endif
										)
									{
										done = true;
										break;
									}
								}
								if (done)
								{
									break;
								}
							}
							x[u] = i;
							x[v] = j;
							du[0] = 0;
							du[1] = 0;
							du[2] = 0;
							du[u] = w;
							dv[0] = 0;
							dv[1] = 0;
							dv[2] = 0;
							dv[v] = h;

							if (backFace)
							{
								r = x[0];
								s = x[1];
								t = x[2];
							}
							else
							{
								r = x[0] + q[0];
								s = x[1] + q[1];
								t = x[2] + q[2];
							}

							glm::vec3 vec0(r, s, t);
							glm::vec3 vec1(r + du[0], s + du[1], t + du[2]);
							glm::vec3 vec2(r + dv[0], s + dv[1], t + dv[2]);
							glm::vec3 vec3(r + du[0] + dv[0], s + du[1] + dv[1], t + du[2] + dv[2]);

							glm::vec4 color(0.1f, 0.7f, 0.1f, 1.0f);

							
#if BAKED_AO
							switch (face) 
							{
							case VoxelFace::Face::TOP:
								WriteQuad(vertices, indices, vec1, vec3, vec0, vec2, color, face, backFace, ao[n]);
								break;
							case VoxelFace::Face::BOTTOM:
								WriteQuad(vertices, indices, vec3, vec1, vec2, vec0, color, face, backFace, ao[n]);
								break;
							case VoxelFace::Face::LEFT:
								WriteQuad(vertices, indices, vec0, vec2, vec1, vec3, color, face, backFace, ao[n]);
								break;
							case VoxelFace::Face::RIGHT:
								WriteQuad(vertices, indices, vec2, vec0, vec3, vec1, color, face, backFace, ao[n]);
								break;
							case VoxelFace::Face::FRONT:
								WriteQuad(vertices, indices, vec0, vec1, vec2, vec3, color, face, backFace, ao[n]);
								break;
							case VoxelFace::Face::BACK:
								WriteQuad(vertices, indices, vec1, vec0, vec3, vec2, color, face, backFace, ao[n]);
								break;
							}
#else
							switch (face)
							{
							case VoxelFace::Face::TOP:
								WriteQuad(vertices, indices, vec1, vec3, vec0, vec2, color, face, backFace);
								break;
							case VoxelFace::Face::BOTTOM:
								WriteQuad(vertices, indices, vec3, vec1, vec2, vec0, color, face, backFace);
								break;
							case VoxelFace::Face::LEFT:
								WriteQuad(vertices, indices, vec0, vec2, vec1, vec3, color, face, backFace);
								break;
							case VoxelFace::Face::RIGHT:
								WriteQuad(vertices, indices, vec2, vec0, vec3, vec1, color, face, backFace);
								break;
							case VoxelFace::Face::FRONT:
								WriteQuad(vertices, indices, vec0, vec1, vec2, vec3, color, face, backFace);
								break;
							case VoxelFace::Face::BACK:
								WriteQuad(vertices, indices, vec1, vec0, vec3, vec2, color, face, backFace);
								break;
							}
#endif

							// Clear the mask
							for (l = 0; l < h; ++l)
							{
								for (k = 0; k < w; ++k)
								{
									mask[n + k + l * CHUNK_SIZE] = (char)0;
#if BAKED_AO
									ao[n + k + l * CHUNK_SIZE] = (char)0;
#endif
								}
							}
							// increment i and n
							i += w;
							n += w;
						}
						else
						{
							i++;
							n++;
						}
					}
				}
			}
		}
	}
	if (indices.size() == 0)
	{
		return NULL;
	}

	std::vector<VertexAttribute> attributes;
	attributes.push_back(VertexAttribute(VERTEX_POSITION_ATTRIB, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, 0));
	attributes.push_back(VertexAttribute(VERTEX_NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, 3));
	attributes.push_back(VertexAttribute(VERTEX_COLOR_ATTRIB, 4, GL_FLOAT, GL_FALSE, VERTEX_SIZE, 6));

	Mesh *pMesh = new Mesh(vertices, indices, attributes);

	return pMesh;

}

const float AOTable[] =
{
	0.25f, 0.50f, 0.75f, 1.0f
};

void GreedyMesher::WriteQuad(std::vector<GLfloat>& vertices, std::vector<GLuint>& indices,
	glm::vec3& bottomLeft, glm::vec3& topLeft, glm::vec3& topRight, glm::vec3& bottomRight, glm::vec4& color, VoxelFace::Face face, bool backFace
#if BAKED_AO
	, char ao
#endif
	)
{

	// Get current vertex count, and add indices to the index list
	int vertCount = vertices.size() / VERTEX_SIZE;

#if BAKED_AO


	int ao0 = CharUtils::GetCharBits(ao, 0, 2);
	int ao1 = CharUtils::GetCharBits(ao, 2, 2);
	int ao2 = CharUtils::GetCharBits(ao, 4, 2);
	int ao3 = CharUtils::GetCharBits(ao, 6, 2);

	float fao0 = AOTable[ao0];
	float fao1 = AOTable[ao1];
	float fao2 = AOTable[ao2];
	float fao3 = AOTable[ao3];

	if (ao0 + ao3 < ao1 + ao2)
	{
		indices.push_back(vertCount + 2);
		indices.push_back(vertCount + 0);
		indices.push_back(vertCount + 1);
		indices.push_back(vertCount + 1);
		indices.push_back(vertCount + 3);
		indices.push_back(vertCount + 2);
	}
	else
	{
		indices.push_back(vertCount + 2);
		indices.push_back(vertCount + 0);
		indices.push_back(vertCount + 3);
		indices.push_back(vertCount + 3);
		indices.push_back(vertCount + 0);
		indices.push_back(vertCount + 1);
	}
		
#else

	indices.push_back(vertCount + 2);
	indices.push_back(vertCount + 0);
	indices.push_back(vertCount + 1);
	indices.push_back(vertCount + 1);
	indices.push_back(vertCount + 3);
	indices.push_back(vertCount + 2);
		
#endif

	// add the face normals
	glm::vec3 normal = VoxelFace::GetOffset(face);

	// Add the verts in the necessary order
	vertices.push_back(bottomLeft.x);
	vertices.push_back(bottomLeft.y);
	vertices.push_back(bottomLeft.z);
	vertices.push_back(normal.x);
	vertices.push_back(normal.y);
	vertices.push_back(normal.z);
	vertices.push_back(color.r
#if BAKED_AO
		* fao0
#endif
		);
	vertices.push_back(color.g
#if BAKED_AO
		* fao0
#endif
		);
	vertices.push_back(color.b
#if BAKED_AO
		* fao0
#endif
		);
	vertices.push_back(color.a);
	
	vertices.push_back(topLeft.x);
	vertices.push_back(topLeft.y);
	vertices.push_back(topLeft.z);
	vertices.push_back(normal.x);
	vertices.push_back(normal.y);
	vertices.push_back(normal.z);
	vertices.push_back(color.r
#if BAKED_AO
		* fao1
#endif
		);
	vertices.push_back(color.g
#if BAKED_AO
		* fao1
#endif
		);
	vertices.push_back(color.b
#if BAKED_AO
		* fao1
#endif
		);
	vertices.push_back(color.a);

	vertices.push_back(topRight.x);
	vertices.push_back(topRight.y);
	vertices.push_back(topRight.z);
	vertices.push_back(normal.x);
	vertices.push_back(normal.y);
	vertices.push_back(normal.z);
	vertices.push_back(color.r
#if BAKED_AO
		* fao2
#endif
		);
	vertices.push_back(color.g
#if BAKED_AO
		* fao2
#endif
		);
	vertices.push_back(color.b
#if BAKED_AO
		* fao2
#endif
		);
	vertices.push_back(color.a);

	vertices.push_back(bottomRight.x);
	vertices.push_back(bottomRight.y);
	vertices.push_back(bottomRight.z);
	vertices.push_back(normal.x);
	vertices.push_back(normal.y);
	vertices.push_back(normal.z);
	vertices.push_back(color.r
#if BAKED_AO
		* fao3
#endif
		);
	vertices.push_back(color.g
#if BAKED_AO
		* fao3
#endif
		);
	vertices.push_back(color.b
#if BAKED_AO
		* fao3
#endif
		);
	vertices.push_back(color.a);

}