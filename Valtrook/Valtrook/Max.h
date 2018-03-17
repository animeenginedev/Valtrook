#pragma once

namespace Val {
	template<class T>
	inline T Max(const T& value, const T& max) {
		if (value > max)
			return value;
		return max;
	}
}