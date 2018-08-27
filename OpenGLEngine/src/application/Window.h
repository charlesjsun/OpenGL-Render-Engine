#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define MAX_KEYS	1024
#define MAX_BUTTONS 32

class Window
{
private:
	const char *m_Title;
	int m_Width, m_Height;
	GLFWwindow *m_pWindow;

	bool m_Keys[MAX_KEYS];
	bool m_MouseButtons[MAX_BUTTONS];

	GLfloat m_MouseX, m_MouseY;
	GLfloat m_LastX, m_LastY;
	GLfloat m_OffsetX, m_OffsetY;

public:
	Window(const char *name, int width, int height);
	~Window();
	void Clear() const;
	void Update();
	bool IsClosed() const;

	bool IsKeyPressed(unsigned int keycode) const;
	bool IsMouseButtonPressed(unsigned int button) const;
	void GetMousePostion(GLfloat& x, GLfloat& y) const;
	void GetMouseOffset(GLfloat& xOffset, GLfloat& yOffset) const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

private:
	bool Init();
	static void WindowResize(GLFWwindow *pWindow, int width, int height);
	static void KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods);
	static void MouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods);
	static void CursorPosCallback(GLFWwindow* pWindow, double xPos, double yPos);

};

