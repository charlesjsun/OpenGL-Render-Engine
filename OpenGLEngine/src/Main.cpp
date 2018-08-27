#include <GL/glew.h>

#include "Game.h"

#include "utils/StringUtils.h"
#include "utils/CharUtils.h"

#define PRINT_MAT4(m) std::cout << m[0][0] << " " << m[1][0] << " " << m[2][0] << " " << m[3][0] << std::endl;\
					  std::cout << m[0][1] << " " << m[1][1] << " " << m[2][1] << " " << m[3][1] << std::endl;\
					  std::cout << m[0][2] << " " << m[1][2] << " " << m[2][2] << " " << m[3][2] << std::endl;\
					  std::cout << m[0][3] << " " << m[1][3] << " " << m[2][3] << " " << m[3][3] << std::endl;

#define PRINT_VEC3(v) std::cout << v.x << " " << v.y << " " << v.z << std::endl;

#define PRINT(x) std::cout << x;
#define PRINTLN(x) std::cout << x << std::endl;

int main()
{
#if 0
	std::cout.setf(std::ios::fixed, std::ios::floatfield);
	std::cout.precision(5);

	glm::mat4 projection = glm::perspective(65.0f, 17.0f / 9.0f, 0.1f, 1000.f);
	glm::mat4 inverseprojection = glm::inverse(projection);
	glm::mat4 inverseprojection2 = getInverseProjection(projection);

	PRINTLN("Projection:");
	PRINT_MAT4(projection);
	PRINTLN("\nInverse Projection:");
	PRINT_MAT4(inverseprojection);
	PRINTLN("\nInverse Projection (Manual):");
	PRINT_MAT4(inverseprojection2);

	glm::mat4 view = glm::lookAt(glm::vec3(3, 2, 3), glm::vec3(4, 1, 2), glm::vec3(0, 1, 0));
	glm::mat4 inverseview = glm::lookAt(glm::vec3(3, 2, 3), glm::vec3(4, 1, 2), glm::vec3(2, 1, 3));

	PRINTLN("\n\nView:");
	PRINT_MAT4(view);
	PRINTLN("\nInverse View:");
	PRINT_MAT4(inverseview);

	glm::mat4 viewprojection = projection * view;
	glm::mat4 inverseviewprojection = glm::inverse(viewprojection);

	PRINTLN("\n\nView Projection:");
	PRINT_MAT4(viewprojection);
	PRINTLN("\nInverse View Projection:");
	PRINT_MAT4(inverseviewprojection);

	glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(2, 3, 4));
	glm::mat4 rotation = glm::rotate(45.0f, glm::vec3(1, 1, 1));
	glm::mat4 scale = glm::scale(glm::vec3(4, 3, 2));

	PRINTLN("\n\nModel:");
	PRINT_MAT4(rotation);

	glm::vec3 normal = glm::normalize(glm::vec3(1, 1, 0.5f));

	PRINTLN("\n\nNormal:");
	PRINT_VEC3(normal);

	glm::vec4 translatedNormal = scale * glm::vec4(normal, 0.0f);

	PRINTLN("\n\nNormal After Model:");
	PRINT_VEC3(glm::normalize(translatedNormal));

	system("PAUSE");
	return 0;
#endif

#if 0

	char c = 0;

	CharUtils::SetCharBits(c, 2, 2, 3);
	
	int i = CharUtils::GetCharBits(c, 0, 2);

	PRINTLN(i);

	system("PAUSE");
	return 0;

#endif

#define WINDOW_WIDTH	960
#define WINDOW_HEIGHT	540
//#define WINDOW_WIDTH	1280
//#define WINDOW_HEIGHT	720

	Game *pGame = Game::GetInstance();
	if (!pGame->Init(WINDOW_WIDTH, WINDOW_HEIGHT))
	{
		system("PAUSE");
		return 0;
	}
	
	while (!pGame->ShouldClose())
	{
		pGame->Update();
		pGame->Render();
	}

	Game::DestroyInstance();

	//system("PAUSE");

	return 0;

}
