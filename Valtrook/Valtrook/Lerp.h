#pragma once

namespace Val {
	template<typename T>
	inline T lerp(const T& a, const T& b, const T& percent) {
		return a + ((b - a) * percent); 
	}

	template<typename In, typename Real>
	inline Real lerp(const In& a, const In& b, const Real& percent) {
		return static_cast<Real>(a + ((b - a)) * percent);
	}
}