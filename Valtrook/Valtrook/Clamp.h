#pragma once

namespace Val {
	template<class T>
	constexpr T Clamp(const T& value, const T& min, const T& max) {
		if (value < min)
			return min;
		if (value > max)
			return max;
		return value;
	}
}