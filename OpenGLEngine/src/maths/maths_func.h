#pragma once

#define _USE_MATH_DEFINES

#include <math.h>

namespace maths {

	inline float toRadians(float degrees)
	{
		return (float) (degrees * M_PI / 180);
	}

}