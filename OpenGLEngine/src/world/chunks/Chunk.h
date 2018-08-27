#pragma once

#define CHUNK_SIZE 32
#define X_SHIFT 5
#define Y_SHIFT 10
#define Z_SHIFT 0

struct ChunkCoord {
	int x;
	int y;
	int z;
};

inline bool const operator==(const ChunkCoord& l, const ChunkCoord& r) {
	return l.x == r.x && l.y == r.y && l.z == r.z;
};

inline bool const operator<(const ChunkCoord& l, const ChunkCoord& r) {
	if (l.x < r.x)  return true;
	if (l.x > r.x)  return false;

	if (l.y < r.y)  return true;
	if (l.y > r.y)  return false;

	if (l.z < r.z)  return true;
	if (l.z > r.z)  return false;

	return false;
};

class Chunk
{
private:


public:
	Chunk();
	~Chunk();

};