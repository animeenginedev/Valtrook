#pragma once

#include <math.h>

namespace Val {
	template<typename T>
	constexpr T DistanceBetweenTwoPoints(const T& x1, const T& y1, const T& x2, const T& y2) {
		return std::sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
	}
	template<typename T>
	constexpr T DistanceBetweenTwoPointsSquared(const T& x1, const T& y1, const T& x2, const T& y2) {
		return ((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1));
	}
}