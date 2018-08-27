#pragma once

#include <iostream>

namespace maths {

	struct vec2
	{

		float x, y;

		vec2();
		vec2(const float& x, const float& y);

		// Add
		vec2& add(const vec2& other);
		friend vec2 operator+(vec2 left, const vec2& right);
		vec2& operator+=(const vec2& other);
		
		// Subtract
		vec2& sub(const vec2& other);
		friend vec2 operator-(vec2 left, const vec2& right);
		vec2& operator-=(const vec2& other);
		
		// Multiply
		vec2& mul(const vec2& other);
		friend vec2 operator*(vec2 left, const vec2& right);
		vec2& operator*=(const vec2& other);
		
		// Divide
		vec2& div(const vec2& other);
		friend vec2 operator/(vec2 left, const vec2& right);
		vec2& operator/=(const vec2& other);

		// Comparison
		bool operator==(const vec2& other);
		bool operator!=(const vec2& other);

		// String output
		friend std::ostream& operator<<(std::ostream& stream, const vec2& vec);

	};

}