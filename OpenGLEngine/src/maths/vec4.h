#pragma once

#include <iostream>

namespace maths {

	struct vec4
	{

		float x, y, z, w;

		vec4();
		vec4(const float& x, const float& y, const float& z, const float& w);

		// Add
		vec4& add(const vec4& other);
		friend vec4 operator+(vec4 left, const vec4& right);
		vec4& operator+=(const vec4& other);
		
		// Subtract
		vec4& sub(const vec4& other);
		friend vec4 operator-(vec4 left, const vec4& right);
		vec4& operator-=(const vec4& other);

		// Multiply
		vec4& mul(const vec4& other);
		friend vec4 operator*(vec4 left, const vec4& right);
		vec4& operator*=(const vec4& other);

		// Divide
		vec4& div(const vec4& other);
		friend vec4 operator/(vec4 left, const vec4& right);
		vec4& operator/=(const vec4& other);

		// Comparison
		bool operator==(const vec4& other);
		bool operator!=(const vec4& other);

		// String output
		friend std::ostream& operator<<(std::ostream& stream, const vec4& vec);

	};

}