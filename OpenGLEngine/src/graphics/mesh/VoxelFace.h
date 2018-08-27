#pragma once

#include <glm/glm.hpp>

class VoxelFace
{
public:

	enum Face
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		FRONT,
		BACK
	};
	
	static glm::ivec3 GetOffset(Face voxelFace)
	{
		switch (voxelFace)
		{
		case TOP:
			return glm::ivec3(0, 1, 0);
		case BOTTOM:
			return glm::ivec3(0, -1, 0);
		case LEFT:
			return glm::ivec3(-1, 0, 0);
		case RIGHT:
			return glm::ivec3(1, 0, 0);
		case FRONT:
			return glm::ivec3(0, 0, 1);
		case BACK:
			return glm::ivec3(0, 0, -1);
		}
	}

private:
	VoxelFace();
	
};