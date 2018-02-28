#pragma once

#include <vector>

class Colour {
public:
	Colour();
	Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	~Colour();

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	Colour add(const Colour& right, bool allowOverflow = false);
	Colour subtract(const Colour& right, bool allowUnderflow = false);
	Colour multiply(const float& right, bool allowUnderOverflow = false);
	Colour divide(const float& right, bool allowUnderOverFlow = false);
		
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