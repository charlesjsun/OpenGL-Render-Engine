#include "Game.h"

//.float CalcPointLightBSphere(const PointLight& Light)
//.{
//.	const GLfloat maxBrightness = std::fmaxf(std::fmaxf(Light.Diffuse.r, Light.Diffuse.g), Light.Diffuse.b);
//.	GLfloat radius = (-Light.Linear + std::sqrtf(Light.Linear * Light.Linear - 4 * Light.Quadratic * (Light.Constant - (256.0f / 5.0f) * maxBrightness))) / (2 * Light.Quadratic);
//.	return radius;
//.}

//float CalcPointLightBSphere(const PointLight& Light)
//{
//	const GLfloat maxBrightness = std::fmaxf(std::fmaxf(Light.Diffuse.r, Light.Diffuse.g), Light.Diffuse.b);
//	GLfloat radius = (std::sqrtf(4 * Light.Quadratic * (Light.Constant - (1 - maxBrightness)))) / (2 * Light.Quadratic);
//	return radius;
//}

//float CalcPointLightBSphere(const PointLight& Light)
//{
//	const GLfloat maxBrightness = std::fmaxf(std::fmaxf(Light.Diffuse.r, Light.Diffuse.g), Light.Diffuse.b);
//	GLfloat radius = maxBrightness / Light.Linear;
//	return radius;
//}

void Game::Render()
{

	

	m_pGeometryPass->Render();


#if !BAKED_AO
	m_pSSAOPass->Render();
#endif

	//m_PointLights[0]->Position = m_pCamera->GetPosition();

	// Enable Blend for Lighting Passes
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	m_pPointLightPass->Render();
	m_pDirectionalLightPass->Render();

	glDisable(GL_BLEND);
	// Finish Lighting Passes

	m_pBloomPass->Render();

	m_pHDRPass->Render();

}
