#include "Material.h"

MaterialManager::MaterialManager()
{
	m_pMaterials = new Material[MAX_MATERIALS];
}

MaterialManager::~MaterialManager()
{
	delete[] m_pMaterials;
	delete m_pMaterialsTexture;

}

bool MaterialManager::Init()
{

	m_pMaterials[Material::LIGHT] = Material(5.0f, 0, 0, 1);
	m_pMaterials[Material::GRASS] = Material(0, 0.1f, 2, 1);

	m_pMaterialsTexture = new Texture1D(TEXTURE_UNIT_MATERIALS, TEX_RGBA32F, MAX_MATERIALS, &m_pMaterials[0]);
	m_pMaterialsTexture->Init();

	return true;

}

void MaterialManager::BindMaterialsTexture()
{
	m_pMaterialsTexture->Bind();
}