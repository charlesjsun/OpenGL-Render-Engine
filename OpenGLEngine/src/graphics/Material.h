#pragma once

#include <GL/glew.h>

#include "textures/Texture1D.h"

struct Material
{

	static enum Types
	{
		LIGHT = 0,
		GRASS,
	};

	float LightEmission;
	float SpecularIntensity;
	float Shininess;
	float Transparency;

	Material()
	{
		LightEmission = 0.0f;
		SpecularIntensity = 0.0f;
		Shininess = 0.0f;
		Transparency = 1.0f;
	}

	Material(float lightEmission, float specularIntensity, float shininess, float transparency)
		: LightEmission(lightEmission), SpecularIntensity(specularIntensity), Shininess(shininess), Transparency(transparency)
	{};

};


#define MAX_MATERIALS 256

class MaterialManager
{
private:
	Material *m_pMaterials;
	Texture1D *m_pMaterialsTexture;

public:
	MaterialManager();
	~MaterialManager();

	bool Init();

	void BindMaterialsTexture();


};

