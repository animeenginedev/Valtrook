#pragma once

namespace Val {
	template<class T>
	inline T Min(const T& value, const T& min) {
		if (value < min)
			return min;
		return value;
	}
}