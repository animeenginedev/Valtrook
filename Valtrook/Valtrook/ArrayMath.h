#pragma once

#include <type_traits>
#include <array>

namespace Val {
	//Array + Array
	template<typename Type, unsigned int size>
	std::array<Type, size> operator+(const std::array<Type, size>& lhs, const std::array<Type, size>& rhs) {
		std::array<Type, size> arr;
		for (unsigned int i = 0; i < size; ++i) {
			arr[i] = lhs[i] + rhs[i];
		}
		return arr;
	}
	template<typename Type, unsigned int size>
	std::array<Type, size> operator-(const std::array<Type, size>& lhs, const std::array<Type, size>& rhs) {
		std::array<Type, size> arr;
		for (unsigned int i = 0; i < size; ++i) {
			arr[i] = lhs[i] - rhs[i];
		}
		return arr;
	}
	template<typename Type, unsigned int size>
	std::array<Type, size> operator*(const std::array<Type, size>& lhs, const std::array<Type, size>& rhs) {
		std::array<Type, size> arr;
		for (unsigned int i = 0; i < size; ++i) {
			arr[i] = lhs[i] * rhs[i];
		}
		return arr;
	}
	template<typename Type, unsigned int size>
	std::array<Type, size> operator/(const std::array<Type, size>& lhs, const std::array<Type, size>& rhs) {
		std::array<Type, size> arr;
		for (unsigned int i = 0; i < size; ++i) {
			arr[i] = lhs[i] / rhs[i];
		}
		return arr;
	}
	//Array + <Value> 
	template<typename Type, unsigned int size, typename Real, 
	typename = std::enable_if_t<!std::is_array<Real>::value>
	>
	std::array<Type, size> operator+(const std::array<Type, size>& lhs, const Real& rhs) {
		std::array<Type, size> arr;
		for (unsigned int i = 0; i < size; ++i) {
			arr[i] = lhs[i] + rhs;
		}
		return arr;
	}
	template<typename Type, unsigned int size, typename Real,
		typename = std::enable_if_t<!std::is_array<Real>::value>
	>
		std::array<Type, size> operator-(const std::array<Type, size>& lhs, const Real& rhs) {
		std::array<Type, size> arr;
		for (unsigned int i = 0; i < size; ++i) {
			arr[i] = lhs[i] - rhs;
		}
		return arr;
	}
	template<typename Type, unsigned int size, typename Real,
		typename = std::enable_if_t<!std::is_array<Real>::value>
	>
		std::array<Type, size> operator*(const std::array<Type, size>& lhs, const Real& rhs) {
		std::array<Type, size> arr;
		for (unsigned int i = 0; i < size; ++i) {
			arr[i] = lhs[i] * rhs;
		}
		return arr;
	}
	template<typename Type, unsigned int size, typename Real,
		typename = std::enable_if_t<!std::is_array<Real>::value>
	>
		std::array<Type, size> operator/(const std::array<Type, size>& lhs, const Real& rhs) {
		std::array<Type, size> arr;
		for (unsigned int i = 0; i < size; ++i) {
			arr[i] = lhs[i] / rhs;
		}
		return arr;
	}
	//
}