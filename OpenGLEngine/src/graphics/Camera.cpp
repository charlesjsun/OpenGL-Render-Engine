#include "Camera.h"

Camera::Camera(glm::vec3& Position, glm::vec3& Up, GLfloat Yaw, GLfloat Pitch, GLfloat Roll)
{
	m_Position = Position;
	m_Front = (glm::vec3(0.0f, 0.0f, -1.0f));
	m_WorldUp = Up;
	m_Yaw = Yaw;
	m_Pitch = Pitch;
	m_Roll = Roll;
	m_MovementSpeed = SPEED;
	m_MouseSensitivity = SENSITIVTY;
	m_Zoom = ZOOM;

	this->UpdateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch, GLfloat roll)
{
	m_Position = glm::vec3(posX, posY, posZ);
	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_WorldUp = glm::vec3(upX, upY, upZ);
	m_Yaw = yaw;
	m_Pitch = pitch;
	m_Roll = roll;
	m_MovementSpeed = SPEED;
	m_MouseSensitivity = SENSITIVTY;
	m_Zoom = ZOOM;

	this->UpdateCameraVectors();
}

void Camera::SetWindow(Window *pWindow)
{
	m_pWindow = pWindow;
	m_ProjectionMatrix = glm::perspective(m_Zoom, (GLfloat)m_pWindow->GetWidth() / (GLfloat)m_pWindow->GetHeight(), NEAR_PLANE, FAR_PLANE);
	m_InverseProjectionMatrix = InvertProjectionMatrix(m_ProjectionMatrix);
}

void Camera::Update(float DeltaTime)
{

	if (m_pWindow->IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		m_MovementSpeed = SPEED * 50;
	else
		m_MovementSpeed = SPEED;

	if (m_pWindow->IsKeyPressed(GLFW_KEY_W))
		ProcessKeyboard(DeltaTime, CameraMovement::FORWARD);
	if (m_pWindow->IsKeyPressed(GLFW_KEY_S))
		ProcessKeyboard(DeltaTime, CameraMovement::BACKWARD);
	if (m_pWindow->IsKeyPressed(GLFW_KEY_A))
		ProcessKeyboard(DeltaTime, CameraMovement::LEFT);
	if (m_pWindow->IsKeyPressed(GLFW_KEY_D))
		ProcessKeyboard(DeltaTime, CameraMovement::RIGHT);
	if (m_pWindow->IsKeyPressed(GLFW_KEY_SPACE))
		ProcessKeyboard(DeltaTime, CameraMovement::UP);
	if (m_pWindow->IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		ProcessKeyboard(DeltaTime, CameraMovement::DOWN);

	if (m_pWindow->IsKeyPressed(GLFW_KEY_Q))
	{
		m_Roll += m_MovementSpeed * 10 * DeltaTime;
		UpdateCameraVectors();
	}
	if (m_pWindow->IsKeyPressed(GLFW_KEY_E))
	{
		m_Roll -= m_MovementSpeed * 10 * DeltaTime;
		UpdateCameraVectors();
	}

	if (m_pWindow->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		GLfloat xOffset;
		GLfloat yOffset;
		m_pWindow->GetMouseOffset(xOffset, yOffset);
		ProcessMouseMovement(xOffset, yOffset);
	}

	//std::cout << "(" << m_Position.x << "," << m_Position.y << "," << m_Position.z << "), (" << m_Front.x << "," << m_Front.y << "," << m_Front.z << ")" << std::endl;
}

void Camera::ProcessKeyboard(GLfloat deltaTime, CameraMovement direction)
{
	GLfloat velocity = m_MovementSpeed * deltaTime;
	if (direction == CameraMovement::FORWARD)
		m_Position += m_Front * velocity;
	if (direction == CameraMovement::BACKWARD)
		m_Position -= m_Front * velocity;
	if (direction == CameraMovement::LEFT)
		m_Position -= m_Right * velocity;
	if (direction == CameraMovement::RIGHT)
		m_Position += m_Right * velocity;
	if (direction == CameraMovement::UP)
		m_Position += m_Up * velocity;
	if (direction == CameraMovement::DOWN)
		m_Position -= m_Up * velocity;

	UpdateViewMatrix();
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_Yaw += xoffset;
	m_Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
	if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
		m_Zoom -= yoffset;
	if (m_Zoom <= 1.0f)
		m_Zoom = 1.0f;
	if (m_Zoom >= 70.0f)
		m_Zoom = 70.0f;
}

void Camera::UpdateCameraVectors()
{
	// Calculate the new Front vector
	m_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front.y = sin(glm::radians(m_Pitch));
	m_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(m_Front);
	// Also re-calculate the Right and Up vector
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));

	UpdateViewMatrix();

}

void Camera::UpdateViewMatrix()
{
	m_ViewMatrix = LookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::LookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& worldUp)
{
	glm::vec3 zaxis = glm::normalize(position - target);
	glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
	glm::vec3 yaxis = glm::cross(zaxis, xaxis);

	// Create translation and rotation matrix
	// In glm we access elements as mat[col][row] due to column-major layout
	glm::mat4 translation; // Identity matrix by default
	translation[3][0] = -position.x; // Third column, first row
	translation[3][1] = -position.y;
	translation[3][2] = -position.z;
	glm::mat4 rotation;
	rotation[0][0] = xaxis.x; // First column, first row
	rotation[1][0] = xaxis.y;
	rotation[2][0] = xaxis.z;
	rotation[0][1] = yaxis.x; // First column, second row
	rotation[1][1] = yaxis.y;
	rotation[2][1] = yaxis.z;
	rotation[0][2] = zaxis.x; // First column, third row
	rotation[1][2] = zaxis.y;
	rotation[2][2] = zaxis.z;

	// Return lookAt matrix as combination of translation and rotation matrix
	return rotation * translation; // Remember to read from right to left (first translation then rotation)
}

glm::mat4 Camera::InvertProjectionMatrix(const glm::mat4& projection)
{
	glm::mat4 inverted;

	inverted[0][0] = 1.0f / projection[0][0];
	inverted[1][1] = 1.0f / projection[1][1];
	inverted[2][3] = 1.0f / projection[3][2];
	inverted[2][2] = 0.0f;
	inverted[3][2] = -1.0f;
	inverted[3][3] = projection[2][2] / projection[3][2];

	return inverted;
}