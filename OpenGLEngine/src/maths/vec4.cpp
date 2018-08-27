#include "vec4.h"

namespace maths {

	vec4::vec4()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}

	vec4::vec4(const float& x, const float& y, const float& z, const float& w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	// Add
	vec4& vec4::add(const vec4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	vec4 operator+(vec4 left, const vec4& right)
	{
		return left.add(right);
	}

	vec4& vec4::operator+=(const vec4& right)
	{
		return add(right);
	}

	// Subtract
	vec4& vec4::sub(const vec4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	vec4 operator-(vec4 left, const vec4& right)
	{
		return left.sub(right);
	}

	vec4& vec4::operator-=(const vec4& right)
	{
		return sub(right);
	}

	// Multiply
	vec4& vec4::mul(const vec4& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}

	vec4 operator*(vec4 left, const vec4& right)
	{
		return left.mul(right);
	}

	vec4& vec4::operator*=(const vec4& right)
	{
		return mul(right);
	}

	// Divide
	vec4& vec4::div(const vec4& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}

	vec4 operator/(vec4 left, const vec4& right)
	{
		return left.div(right);
	}

	vec4& vec4::operator/=(const vec4& right)
	{
		return div(right);
	}

	// Comparison
	bool vec4::operator==(const vec4& other)
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	bool vec4::operator!=(const vec4& other)
	{
		return x != other.x || y != other.y || z != other.z || w != other.w;
	}

	// String Output
	std::ostream& operator<<(std::ostream& stream, const vec4& vec)
	{
		stream << "vec4: (" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
		return stream;
	}

}