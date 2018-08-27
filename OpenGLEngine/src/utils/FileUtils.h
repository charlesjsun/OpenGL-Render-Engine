#pragma once

#include <iostream>

class FileUtils
{
public:
	static std::string ReadFile(const char *filepath)
	{
		FILE *pFile = fopen(filepath, "rt");

		if (pFile == NULL)
		{
			std::cout << "COULD NOT READ FILE: " << filepath << std::endl;
			return NULL;
		}

		fseek(pFile, 0, SEEK_END);
		unsigned long length = ftell(pFile);
		char *data = new char[length + 1];
		memset(data, 0, length + 1);
		fseek(pFile, 0, SEEK_SET);
		fread(data, 1, length, pFile);
		fclose(pFile);

		std::string result(data);
		delete[] data;
		return result;
	}

private:
	FileUtils();

};