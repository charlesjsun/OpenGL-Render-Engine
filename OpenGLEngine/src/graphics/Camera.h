#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../application/Window.h"

enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

// Default camera values
#define YAW -90.0f
#define PITCH 0.0f
#define ROLL 0
#define SPEED 5.0f
#define SENSITIVTY 0.25f
#define ZOOM 70.0f


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
private:
	Window *m_pWindow;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_InverseProjectionMatrix;
	glm::mat4 m_ViewMatrix;

	// Camera Attributes
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;
	// Eular Angles
	GLfloat m_Yaw;
	GLfloat m_Pitch;
	GLfloat m_Roll;
	// Camera options
	GLfloat m_MovementSpeed;
	GLfloat m_MouseSensitivity;
	GLfloat m_Zoom;

public:
	Camera(glm::vec3& Position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3& Up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat Yaw = YAW, GLfloat Pitch = PITCH, GLfloat Roll = ROLL);
	Camera(GLfloat PosX, GLfloat PosY, GLfloat PosZ, GLfloat UpX, GLfloat UpY, GLfloat UpZ, GLfloat Yaw, GLfloat Pitch, GLfloat Roll);
	
	void SetWindow(Window *pWindow);

	void Update(float DeltaTime);

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(GLfloat deltaTime, CameraMovement direction);
	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yoffset);

	// - GETTERS
	inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
	inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	inline const glm::mat4& GetInverseProjectionMatrix() const { return m_InverseProjectionMatrix; }
	inline const glm::vec3& GetPosition() const { return m_Position; }
	
	inline GLfloat GetYaw() const { return m_Yaw; }
	inline GLfloat GetPitch() const { return m_Pitch; }
	inline GLfloat GetFOV() const { return m_Zoom; }


private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void UpdateCameraVectors();

	void UpdateViewMatrix();

	// Calculate the view matrix
	glm::mat4 LookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& worldUp);
	glm::mat4 InvertProjectionMatrix(const glm::mat4& projection);

};