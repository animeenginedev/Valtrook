#pragma once

#include <math.h>
#include <type_traits>

namespace Val {
	//If you want to verbosly label things
	typedef int Pixel;
	typedef float Meter;
	typedef double D_Meter;

	template<typename Integer, typename Real,
		typename = std::enable_if_t < std::is_integral<Integer>::value >,
		typename = std::enable_if_t< std::is_floating_point<Real>::value> >
	constexpr const Real PixelToWorld(const Integer& pixels) {
		return static_cast<Real>(pixels) * static_cast<Real>(0.01);
	}

	template<typename Real,
		typename = std::enable_if_t< std::is_floating_point<Real>::value> >
	constexpr const Real UnalignedPixelToWorld(const Real& pixels) {
		return static_cast<Real>(pixels) * static_cast<Real>(0.01);
	}

	template<typename Integer, typename Real,
		typename = std::enable_if_t < std::is_integral<Integer>::value >,
		typename = std::enable_if_t< std::is_floating_point<Real>::value> >
	constexpr const Integer WorldToPixel(const Real& meters) {
		return static_cast<Integer>(floor(meters * static_cast<Real>(100.0)));
	}

	template<typename Real,
		typename = std::enable_if_t< std::is_floating_point<Real>::value> >
	constexpr const Real WorldToUnalignedPixel(const Real& meters) {
		return meters * static_cast<Real>(100.0);
	}
}