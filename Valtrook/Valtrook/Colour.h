#pragma once

#include <vector>

namespace Val {
	class Colour;

	struct TrueColour {
		TrueColour();
		TrueColour(int r, int g, int b, int a);
		TrueColour(Colour c);

		int r, g, b, a;
	};

	class Colour {
	public:
		Colour();
		Colour(TrueColour colour);
		Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
		~Colour();

		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

		Colour add(const Colour& right, bool allowOverflow = false) const;
		Colour subtract(const Colour& right, bool allowUnderflow = false) const;
		Colour multiply(const float& right, bool allowUnderOverflow = false) const;
		Colour divide(const float& right, bool allowUnderOverFlow = false) const;

		template<typename Real = float,
			typename = std::enable_if_t<std::is_floating_point<Real>::value>>
		static Colour lerp(Colour start, Colour end, Real t) {
			return Colour(static_cast<unsigned char>(static_cast<Real>(start.r + ((end.r - start.r)) * t)),
						  static_cast<unsigned char>(static_cast<Real>(start.g + ((end.g - start.g)) * t)),
						  static_cast<unsigned char>(static_cast<Real>(start.b + ((end.b - start.b)) * t)),
						  static_cast<unsigned char>(static_cast<Real>(start.a + ((end.a - start.a)) * t)));
		}

		template<typename T>
		Colour operator+ (const T& right);
		template<typename T>
		Colour operator- (const T& right);
		template<typename T>
		Colour operator* (const T& right);
		template<typename T>
		Colour operator/ (const T& right);

		unsigned char& operator[](const std::size_t index);

	};

	bool operator ==(const TrueColour& left, const TrueColour& right);
	bool operator !=(const TrueColour& left, const TrueColour& right);
	TrueColour operator +(const TrueColour& left, const TrueColour& right);
	TrueColour operator -(const TrueColour& left, const TrueColour& right);
	TrueColour operator *(const TrueColour& left, const TrueColour& right);
	TrueColour operator *(const TrueColour& left, const float& right);
	TrueColour operator /(const TrueColour& left, const float& right);
	TrueColour& operator +=(TrueColour& left, const TrueColour& right);
	TrueColour& operator -=(TrueColour& left, const TrueColour& right);
	TrueColour& operator *=(TrueColour& left, const TrueColour& right);

	template<typename T>
	inline Colour Colour::operator+(const T & right) {
		return Colour(r + right, g + right, b + right, a + right);
	}

	template<typename T>
	inline Colour Colour::operator-(const T & right) {
		return Colour(r - right, g - right, b - right, a - right);
	}

	template<typename T>
	inline Colour Colour::operator*(const T & right) {
		return Colour(r * right, g * right, b * right, a * right);
	}

	template<typename T>
	inline Colour Colour::operator/(const T & right) {
		return Colour(r / right, g / right, b / right, a / right);
	}

	template<>
	inline Colour Colour::operator+<Colour>(const Colour & right) {
		return Colour(r + right.r, g + right.g, b + right.b, a + right.a);
	}

	template<>
	inline Colour Colour::operator-<Colour>(const Colour & right) {
		return Colour(r - right.r, g - right.g, b - right.b, a - right.a);
	}

	template<>
	inline Colour Colour::operator*<Colour>(const Colour & right) {
		return Colour(r * right.r, g * right.g, b * right.b, a * right.a);
	}

	template<>
	inline Colour Colour::operator/<Colour>(const Colour & right) {
		return Colour(r / right.r, g / right.g, b / right.b, a / right.a);
	}
}