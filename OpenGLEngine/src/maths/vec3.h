#pragma once

#include <iostream>

namespace maths {

	struct vec3
	{

		float x, y, z;

		vec3();
		vec3(const float& x, const float& y, const float& z);

		// Add
		vec3& add(const vec3& other);
		friend vec3 operator+(vec3 left, const vec3& right);
		vec3& operator+=(const vec3& other);
		
		// Subtract
		vec3& sub(const vec3& other);
		friend vec3 operator-(vec3 left, const vec3& right);
		vec3& operator-=(const vec3& other);
		
		// Multiply
		vec3& mul(const vec3& other);
		friend vec3 operator*(vec3 left, const vec3& right);
		vec3& operator*=(const vec3& other);
		
		// Divide
		vec3& div(const vec3& other);
		friend vec3 operator/(vec3 left, const vec3& right);
		vec3& operator/=(const vec3& other);

		// Comparison
		bool operator==(const vec3& other);
		bool operator!=(const vec3& other);

		// String output
		friend std::ostream& operator<<(std::ostream& stream, const vec3& vec);

	};

}