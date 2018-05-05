#include "Colour.h"

#include <assert.h>

namespace Val {
	Colour::Colour() : r(r), g(g), b(b), a(a) {
	}

	Colour::Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {
	}
	Colour::Colour(TrueColour colour) : r(colour.r < 0 ? 0 : colour.r > 255 ? 255 : colour.r),
		g(colour.g < 0 ? 0 : colour.g > 255 ? 255 : colour.g),
		b(colour.b < 0 ? 0 : colour.b > 255 ? 255 : colour.b),
		a(colour.a < 0 ? 0 : colour.a > 255 ? 255 : colour.a) {
	}



	Colour::~Colour() {
	}

	//theres probably a 200iq way to do this without being a retard but whatever unlucky.

	
	inline unsigned char colourAdd(unsigned char left, unsigned char right, bool allowOverflow) {
		int ret = static_cast<int>(left) + static_cast<int>(right);
		if (!allowOverflow) {
			if (ret < 0)
				return 0;
			if (ret > static_cast<int>(INT8_MAX))
				return static_cast<unsigned char>(INT8_MAX);
		}
		return static_cast<unsigned char>(ret);
	}

	inline unsigned char colourTake(unsigned char left, unsigned char right, bool allowOverflow) {
		int ret = static_cast<int>(left) - static_cast<int>(right);
		if (!allowOverflow) {
			if (ret < 0)
				return 0;
			if (ret > static_cast<int>(INT8_MAX))
				return static_cast<unsigned char>(INT8_MAX);
		}
		return static_cast<unsigned char>(ret);
	}

	inline unsigned char colourMultiply(unsigned char left, float right, bool overUnderFlow) {
		unsigned int ret = static_cast<unsigned int>(static_cast<unsigned int>(left) * right);
		if (!overUnderFlow) {
			if (ret < 0)
				return 0;
			if (ret > static_cast<int>(INT8_MAX))
				return static_cast<unsigned char>(INT8_MAX);
		}
		return static_cast<unsigned char>(ret);
	}

	inline unsigned char colourDivide(unsigned char left, float right, bool overUnderFlow) {
		unsigned int ret = static_cast<unsigned int>(static_cast<unsigned int>(left) / right);
		if (!overUnderFlow) {
			if (ret < 0)
				return 0;
			if (ret > static_cast<int>(INT8_MAX))
				return static_cast<unsigned char>(INT8_MAX);
		}
		return static_cast<unsigned char>(ret);
	}



	Colour Colour::add(const Colour & right, bool allowOverflow) const {
		return Colour(colourAdd(r, right.r, allowOverflow),
					  colourAdd(g, right.g, allowOverflow),
					  colourAdd(b, right.b, allowOverflow),
					  colourAdd(a, right.a, allowOverflow));
	}
	Colour Colour::subtract(const Colour & right, bool allowUnderflow) const {
		return Colour(colourTake(r, right.r, allowUnderflow),
					  colourTake(g, right.g, allowUnderflow),
					  colourTake(b, right.b, allowUnderflow),
					  colourTake(a, right.a, allowUnderflow));
	}
	Colour Colour::multiply(const float & right, bool allowUnderOverflow) const {
		return Colour(colourMultiply(r, right, allowUnderOverflow),
					  colourMultiply(g, right, allowUnderOverflow),
					  colourMultiply(b, right, allowUnderOverflow),
					  colourMultiply(a, right, allowUnderOverflow));
	}
	Colour Colour::divide(const float & right, bool allowUnderOverflow) const {
		return Colour(colourDivide(r, right, allowUnderOverflow),
					  colourDivide(g, right, allowUnderOverflow),
					  colourDivide(b, right, allowUnderOverflow),
					  colourDivide(a, right, allowUnderOverflow));
	}

	unsigned char & Colour::operator[](const std::size_t index) {
		switch (index) {
			case 0:
				return r;
			case 1:
				return g;
			case 2:
				return b;
			case 3:
				return a;
		}
		return a;
	}

	bool operator==(const TrueColour & left, const TrueColour & right) {
		return (left.r == right.r &&
				left.g == right.g &&
				left.b == right.b &&
				left.a == right.a);
	}

	bool operator!=(const TrueColour & left, const TrueColour & right) {
		return (left.r != right.r ||
				left.g != right.g ||
				left.b != right.b ||
				left.a != right.a);
	}

	TrueColour operator+(const TrueColour & left, const TrueColour & right) {
		return TrueColour(left.r + right.r,
						  left.g + right.g,
						  left.b + right.b,
						  left.a + right.a);
	}

	TrueColour operator-(const TrueColour & left, const TrueColour & right) {
		return TrueColour(left.r - right.r,
						  left.g - right.g,
						  left.b - right.b,
						  left.a - right.a);
	}

	TrueColour operator*(const TrueColour & left, const TrueColour & right) {
		return TrueColour(left.r * right.r,
						  left.g * right.g,
						  left.b * right.b,
						  left.a * right.a);
	}

	TrueColour operator*(const TrueColour & left, const float & right) {
		return TrueColour(left.r * right,
						  left.g * right,
						  left.b * right,
						  left.a * right);
	}
	TrueColour operator/(const TrueColour & left, const float & right) {
		return TrueColour(left.r / right,
						  left.g / right,
						  left.b / right,
						  left.a / right);
	}

	TrueColour& operator+=(TrueColour & left, const TrueColour & right) {
		return left = left + right;
	}

	TrueColour& operator-=(TrueColour & left, const TrueColour & right) {
		return left = left - right;
	}

	TrueColour& operator*=(TrueColour & left, const TrueColour & right) {
		return left = left * right;
	}

	TrueColour::TrueColour() : r(0), g(0), b(0), a(0) {
	}

	TrueColour::TrueColour(int r, int g, int b, int a) : r(r), g(g), b(b), a(a) {
	}

	TrueColour::TrueColour(Colour c) : r(c.r), g(c.g), b(c.b), a(c.a) {
	}

}