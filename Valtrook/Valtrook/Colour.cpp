#include "Colour.h"

#include <assert.h>

Colour::Colour() : r(r), g(g), b(b), a(a) {
}

Colour::Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {
}


Colour::~Colour() {
}

inline unsigned char colourAdd(unsigned char left, unsigned char right, bool overUnderFlow) {
	unsigned int ret = static_cast<unsigned int>(left) + static_cast<unsigned int>(right);
	if (overUnderFlow && ret > static_cast<unsigned int>(INT8_MAX))
		return static_cast<unsigned char>(INT8_MAX);
	return static_cast<unsigned char>(ret);
}

inline unsigned char colourTake(unsigned char left, unsigned char right, bool overUnderFlow) {
	unsigned int ret = static_cast<unsigned int>(left) - static_cast<unsigned int>(right);
	if (overUnderFlow && ret > static_cast<unsigned int>(INT8_MAX))
		return static_cast<unsigned char>(INT8_MAX);
	return static_cast<unsigned char>(ret);
}

inline unsigned char colourMultiply(unsigned char left, float right, bool overUnderFlow) {
	unsigned int ret = static_cast<unsigned int>(static_cast<unsigned int>(left) * right);
	if (overUnderFlow && ret > static_cast<unsigned int>(INT8_MAX))
		return static_cast<unsigned char>(INT8_MAX);
	return static_cast<unsigned char>(ret);
}

inline unsigned char colourDivide(unsigned char left, float right, bool overUnderFlow) {
	unsigned int ret = static_cast<unsigned int>(static_cast<unsigned int>(left) / right);
	if (overUnderFlow && ret > static_cast<unsigned int>(INT8_MAX))
		return static_cast<unsigned char>(INT8_MAX);
	return static_cast<unsigned char>(ret);
}



Colour Colour::add(const Colour & right, bool allowOverflow) {
	return Colour(colourAdd(r, right.r, allowOverflow),
				  colourAdd(g, right.g, allowOverflow),
				  colourAdd(b, right.b, allowOverflow),
				  colourAdd(a, right.a, allowOverflow));
}
Colour Colour::subtract(const Colour & right, bool allowUnderflow) {
	return Colour(colourTake(r, right.r, allowUnderflow),
				  colourTake(g, right.g, allowUnderflow),
				  colourTake(b, right.b, allowUnderflow),
				  colourTake(a, right.a, allowUnderflow));
}
Colour Colour::multiply(const float & right, bool allowUnderOverflow) {
	return Colour(colourMultiply(r, right, allowUnderOverflow),
				  colourMultiply(g, right, allowUnderOverflow),
				  colourMultiply(b, right, allowUnderOverflow),
				  colourMultiply(a, right, allowUnderOverflow));
}
Colour Colour::divide(const float & right, bool allowUnderOverflow) {
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
