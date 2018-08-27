#include "window.h"

Window::Window(const char *title, int width, int height)
{
	m_Title = title;
	m_Width = width;
	m_Height = height;

	m_LastX = m_MouseX;
	m_LastY = m_MouseY;

	for (int i = 0; i < MAX_KEYS; i++)
	{
		m_Keys[i] = false;
	}

	for (int i = 0; i < MAX_BUTTONS; i++)
	{
		m_MouseButtons[i] = false;
	}

	if (!Init())
		glfwTerminate();
}

Window::~Window()
{
	glfwTerminate();
}

bool Window::Init()
{
	if (!glfwInit())
	{
		std::cout << "GLFW failed to initialize!" << std::endl;
		return false;
	}

	m_pWindow = glfwCreateWindow(m_Width, m_Height, m_Title, NULL, NULL);
	if (!m_pWindow)
	{
		std::cout << "Failed to create GLFW Window!!!!!" << std::endl;
		return false;
	}

	glfwMakeContextCurrent(m_pWindow);

	glfwSetWindowUserPointer(m_pWindow, this);
	glfwSetWindowSizeCallback(m_pWindow, WindowResize);
	glfwSetKeyCallback(m_pWindow, KeyCallback);
	glfwSetMouseButtonCallback(m_pWindow, MouseButtonCallback);
	glfwSetCursorPosCallback(m_pWindow, CursorPosCallback);
	//glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSwapInterval(0); 

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW failed to initialize!" << std::endl;
		return false;
	}

	

	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

	return true;

}

void Window::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Update()
{

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		std::cout << "ERROR: " << error << std::endl;

	glfwPollEvents();

	m_OffsetX = m_MouseX - m_LastX;
	m_OffsetY = m_LastY - m_MouseY;

	m_LastX = m_MouseX;
	m_LastY = m_MouseY;

	// Resizing
	glfwGetFramebufferSize(m_pWindow, &m_Width, &m_Height);

	glfwSwapBuffers(m_pWindow);
}

bool Window::IsClosed() const
{
	return glfwWindowShouldClose(m_pWindow) == 1 || IsKeyPressed(GLFW_KEY_ESCAPE);
}

void Window::WindowResize(GLFWwindow *pWindow, int width, int height)
{
	Window *win = (Window*)glfwGetWindowUserPointer(pWindow);
	win->m_Width = width;
	win->m_Height = height;
	glViewport(0, 0, width, height);
}

bool Window::IsKeyPressed(unsigned int keycode) const
{
	if (keycode >= MAX_KEYS)
		return false;

	return m_Keys[keycode];
}

bool Window::IsMouseButtonPressed(unsigned int button) const
{
	if (button >= MAX_BUTTONS)
		return false;

	return m_MouseButtons[button];
}

void Window::GetMousePostion(GLfloat& x, GLfloat& y) const
{
	x = m_MouseX;
	y = m_MouseY;
}

void Window::GetMouseOffset(GLfloat& xOffset, GLfloat& yOffset) const
{
	xOffset = m_OffsetX;
	yOffset = m_OffsetY;
}

void Window::KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
	Window *win = (Window*)glfwGetWindowUserPointer(pWindow);
	win->m_Keys[key] = action != GLFW_RELEASE;
}

void Window::MouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods)
{
	Window *win = (Window*)glfwGetWindowUserPointer(pWindow);
	win->m_MouseButtons[button] = action != GLFW_RELEASE;
}

void Window::CursorPosCallback(GLFWwindow* pWindow, double xPos, double yPos)
{
	Window *win = (Window*)glfwGetWindowUserPointer(pWindow);

	win->m_MouseX = xPos;
	win->m_MouseY = yPos;

}

