#pragma once

namespace Val {
	template<class T>
	constexpr T Min(const T& value, const T& min) {
		if (value < min)
			return min;
		return value;
	}
}