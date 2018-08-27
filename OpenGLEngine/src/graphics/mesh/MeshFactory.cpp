#include "MeshFactory.h"

#include <iostream>

#include "../../utils/FileUtils.h"
#include "../../utils/StringUtils.h"
#include <glm/glm.hpp>


Mesh* MeshFactory::LoadFromOBJ(const char *path)
{
	std::string file = FileUtils::ReadFile(path);

	if (file.empty()) 
	{
		std::cout << "Could not open file " << path << "!!!!" << std::endl;
		return NULL;
	}

	std::vector<std::string> lines = StringUtils::Split(file, '\n');
	
	std::vector<GLuint> indices;
	std::vector<GLfloat> vertices;
	GLuint index = 0;
	
	std::vector<GLfloat> v;
	std::vector<GLfloat> vt;
	std::vector<GLfloat> vn;
	std::unordered_map<std::string, GLuint> f;

	unsigned int lineNum = 0;

	for (std::string sline : lines)
	{
		const char *line = sline.c_str();
		lineNum++;
		if (strcmp(sline.substr(0, 1).c_str(), "#") == 0)
		{
			continue;
		}
		else if (strcmp(sline.substr(0, 1).c_str(), "v") == 0)
		{
			if (strcmp(sline.substr(1, 1).c_str(), "t") == 0)
			{
				float u, v;
				int result = sscanf(line, "%*s %f %f", &u, &v);
				if (result == 0)
				{
					std::cout << path << ": Invalid vertex at line " << lineNum << "!!! Line: " << line << std::endl;
					continue;
				}
				vt.push_back(u);
				vt.push_back(v);
			}
			else if (strcmp(sline.substr(1, 1).c_str(), "n") == 0)
			{
				float x, y, z;
				int result = sscanf(line, "%*s %f %f %f", &x, &y, &z);
				if (result == 0)
				{
					std::cout << path << ": Invalid vertex at line " << lineNum << "!!! Line: " << line << std::endl;
					continue;
				}
				vn.push_back(x);
				vn.push_back(y);
				vn.push_back(z);
			}
			else
			{
				float x, y, z;
				int result = sscanf(line, "%*s %f %f %f", &x, &y, &z);
				if (result == 0)
				{
					std::cout << path << ": Invalid vertex at line " << lineNum << "!!! Line: " << line << std::endl;
					continue;
				}
				v.push_back(x);
				v.push_back(y);
				v.push_back(z);
			}
		}
		else if (strcmp(sline.substr(0, 1).c_str(), "f") == 0)
		{
			std::vector<std::string> values = StringUtils::Split(sline, ' ');
			if (values.size() != 4)
			{
				std::cout << path << ": Invalid index at line " << lineNum << "!!! Line: " << line << std::endl;
				continue;
			}

			for (GLuint i = 1; i <= 3; i++) {
				std::unordered_map<std::string, GLuint>::const_iterator key = f.find(values[i]);
				if (key == f.end())
				{
					bool success = ParseF(v, vt, vn, f, values[i], vertices, index++);
					if (!success)
					{
						std::cout << path << ": Invalid index at line " << lineNum << "!!! Line: " << line << std::endl;
						continue;
					}
				}

				indices.push_back(f[values[i]]);
			}
		}
	}

	std::vector<VertexAttribute> attributes;
	bool useVN = vn.size() != 0;
	bool useVT = vt.size() != 0;
	int stride = 3 + (useVN ? 3 : 0) + (useVT ? 2 : 0);
	int offset = 0;

	attributes.push_back(VertexAttribute(VERTEX_POSITION_ATTRIB, 3, GL_FLOAT, GL_FALSE, stride, offset));
	offset += 3;
	if (useVN)
	{
		attributes.push_back(VertexAttribute(VERTEX_NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE, stride, offset));
		offset += 3;
	}
	if (useVT)
		attributes.push_back(VertexAttribute(VERTEX_TEXTURE_ATTRIB, 2, GL_FLOAT, GL_FALSE, stride, offset));

	Mesh *pMesh = new Mesh(vertices, indices, attributes);
	return pMesh;

}

bool MeshFactory::ParseF(std::vector<GLfloat>& v, std::vector<GLfloat>& vt, std::vector<GLfloat>& vn, std::unordered_map<std::string, GLuint>& f, std::string value, std::vector<GLfloat>& vertices, GLuint index)
{

	std::vector<std::string> values = StringUtils::Split(value, '/');
	int j = 0;

	if (values.size() <= j)
		return false;
	int vID = atoi(values[j++].c_str()) - 1;
	for (int i = 0; i < 3; i++)
		vertices.push_back(v[vID * 3 + i]);

	if (!vn.empty())
	{
		if (values.size() <= j)
			return false;
		int vnID = atoi(values[j++].c_str()) - 1;
		for (int i = 0; i < 3; i++)
			vertices.push_back(vn[vnID * 3 + i]);
	}

	if (!vt.empty())
	{
		if (values.size() <= j)
			return false;
		int vtID = atoi(values[j].c_str()) - 1;
		for (int i = 0; i < 2; i++)
			vertices.push_back(vt[vtID * 2 + i]);
	}

	f[value] = index;
	return true;
}

