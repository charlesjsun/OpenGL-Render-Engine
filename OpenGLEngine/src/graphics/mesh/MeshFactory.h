#pragma once

#include <unordered_map>

#include "Mesh.h"

class MeshFactory 
{
public:
	static Mesh* LoadFromOBJ(const char *path);

private:
	static bool ParseF(std::vector<GLfloat>& v, std::vector<GLfloat>& vt, std::vector<GLfloat>& vn, std::unordered_map<std::string, GLuint>& f, std::string value, std::vector<GLfloat>& vertices, GLuint index);

};