#include "Game.h"

void Game::Update()
{

	// FPS Counter
	GLdouble LastTime = m_CurrentTime;
	m_CurrentTime = glfwGetTime();
	GLfloat DeltaTime = m_CurrentTime - LastTime;

#ifdef DEBUG
	m_Frames++;
	if (m_CurrentTime - m_LastTime >= 1.0)
	{
		printf("%f ms/frame | %d FPS\n", 1000.0 / (double)m_Frames, m_Frames);
		m_Frames = 0;
		m_LastTime += 1.0;
	}
#endif

	m_pCamera->Update(DeltaTime);
	m_pWindow->Update();

}