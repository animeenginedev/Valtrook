#pragma once

//Taken straight from https://github.com/Microsoft/GSL/blob/master/include/gsl/pointers

#include <algorithm>    // for forward
#include <iosfwd>       // for ptrdiff_t, nullptr_t, ostream, size_t
#include <memory>       // for shared_ptr, unique_ptr
#include <system_error> // for hash
#include <type_traits> // for enable_if_t, is_convertible, is_assignable

namespace Val {
	template <class T>
	class Not_Null {
	public:
		static_assert(std::is_assignable<T&, std::nullptr_t>::value, "T cannot be assigned nullptr.");

		template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
		constexpr Not_Null(U&& u) : ptr_(std::forward<U>(u)) {
			Expects(ptr_ != nullptr);
		}

		template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
		constexpr Not_Null(const Not_Null<U>& other) : Not_Null(other.get()) {
		}

		Not_Null(const Not_Null& other) = default;
		Not_Null& operator=(const Not_Null& other) = default;

		constexpr T get() const {
			Ensures(ptr_ != nullptr);
			return ptr_;
		}

		constexpr operator T() const { return get(); }
		constexpr T operator->() const { return get(); }
		constexpr decltype(auto) operator*() const { return *get(); }

		// prevents compilation when someone attempts to assign a null pointer constant
		Not_Null(std::nullptr_t) = delete;
		Not_Null& operator=(std::nullptr_t) = delete;

		// unwanted operators...pointers only point to single objects!
		Not_Null& operator++() = delete;
		Not_Null& operator--() = delete;
		Not_Null operator++(int) = delete;
		Not_Null operator--(int) = delete;
		Not_Null& operator+=(std::ptrdiff_t) = delete;
		Not_Null& operator-=(std::ptrdiff_t) = delete;
		void operator[](std::ptrdiff_t) const = delete;

	private:
		T ptr_;
	};

	template <class T>
	std::ostream& operator<<(std::ostream& os, const Not_Null<T>& val) {
		os << val.get();
		return os;
	}

	template <class T, class U>
	auto operator==(const Not_Null<T>& lhs, const Not_Null<U>& rhs) -> decltype(lhs.get() == rhs.get()) {
		return lhs.get() == rhs.get();
	}

	template <class T, class U>
	auto operator!=(const Not_Null<T>& lhs, const Not_Null<U>& rhs) -> decltype(lhs.get() != rhs.get()) {
		return lhs.get() != rhs.get();
	}

	template <class T, class U>
	auto operator<(const Not_Null<T>& lhs, const Not_Null<U>& rhs) -> decltype(lhs.get() < rhs.get()) {
		return lhs.get() < rhs.get();
	}

	template <class T, class U>
	auto operator<=(const Not_Null<T>& lhs, const Not_Null<U>& rhs) -> decltype(lhs.get() <= rhs.get()) {
		return lhs.get() <= rhs.get();
	}

	template <class T, class U>
	auto operator>(const Not_Null<T>& lhs, const Not_Null<U>& rhs) -> decltype(lhs.get() > rhs.get()) {
		return lhs.get() > rhs.get();
	}

	template <class T, class U>
	auto operator>=(const Not_Null<T>& lhs, const Not_Null<U>& rhs) -> decltype(lhs.get() >= rhs.get()) {
		return lhs.get() >= rhs.get();
	}

	// more unwanted operators
	template <class T, class U>
	std::ptrdiff_t operator-(const Not_Null<T>&, const Not_Null<U>&) = delete;
	template <class T>
	Not_Null<T> operator-(const Not_Null<T>&, std::ptrdiff_t) = delete;
	template <class T>
	Not_Null<T> operator+(const Not_Null<T>&, std::ptrdiff_t) = delete;
	template <class T>
	Not_Null<T> operator+(std::ptrdiff_t, const Not_Null<T>&) = delete; 
}

namespace std {
	template <class T>
	struct hash<Val::Not_Null<T>> {
		std::size_t operator()(const Val::Not_Null<T>& value) const { return hash<T>{}(value); }
	};

}