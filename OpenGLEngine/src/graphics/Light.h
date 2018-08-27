#pragma once

#include <glm/glm.hpp>
#include <string>

class DirectionalLight
{
public:
	glm::vec3 Direction;

	float Ambient;
	glm::vec3 Diffuse;

	DirectionalLight()
	{
		Direction = glm::vec3(0.0f, 0.0f, 0.0f);
		Ambient = 0.0f;
		Diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	DirectionalLight(glm::vec3 direction, float ambient, glm::vec3 diffuse)
		:Direction(direction), Ambient(ambient), Diffuse(diffuse)
	{
	}

	~DirectionalLight()
	{
	}

};


class PointLight
{
public:
	glm::vec3 Position;
	glm::vec3 Diffuse;
	float AttenFactor; // radius ^ 2 / 2


	PointLight()
	{
		Position = glm::vec3(0.0f, 0.0f, 0.0f);
		Diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
		AttenFactor = 0.0f;
	}

	PointLight(glm::vec3 position, glm::vec3 diffuse, float radius)
		: Position(position), Diffuse(diffuse)
	{
		AttenFactor = radius * radius / 4.0f;
	}

	~PointLight()
	{
	}

	void SetRadius(float radius)
	{
		AttenFactor = radius * radius / 4.0f;
	}

	inline float GetRadius()
	{
		return 2 * sqrtf(AttenFactor);
	}

};

#define COLOR_WHITE glm::vec3(1.0f, 1.0f, 1.0f)
#define COLOR_RED glm::vec3(1.0f, 0.0f, 0.0f)
#define COLOR_GREEN glm::vec3(0.0f, 1.0f, 0.0f)
#define COLOR_CYAN glm::vec3(0.0f, 1.0f, 1.0f)
#define COLOR_BLUE glm::vec3(0.0f, 0.0f, 1.0f)