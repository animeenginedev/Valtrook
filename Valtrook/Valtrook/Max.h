#pragma once

namespace Val {
	template<class T>
	constexpr T Max(const T& value, const T& max) {
		if (value > max)
			return max;
		return value;
	}
}