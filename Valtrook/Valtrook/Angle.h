#pragma once

namespace Val {
	template<typename Real>
	inline Real Radians(const Real& degrees) {
		static double piOver180 = (3.141592653589793 / 180.0);
		return degrees * static_cast<Real>(piOver180);
	}
	template<typename Real>
	inline Real Degrees(const Real& radians) {
		static double piUnder180 = (180.0 / 3.141592653589793);
		return radians * static_cast<T>(piUnder180);
	}
}