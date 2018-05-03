#include "Colour.h"

#include <assert.h>

namespace Val {
	void Colour::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<Colour>(), "Colour");

		script->add(chaiscript::constructor<Colour()>(), "Colour");
		script->add(chaiscript::constructor<Colour(unsigned char, unsigned char, unsigned char, unsigned char)>(), "Colour");

		script->add(chaiscript::fun(&Colour::r), "r");
		script->add(chaiscript::fun(&Colour::g), "g");
		script->add(chaiscript::fun(&Colour::b), "b");
		script->add(chaiscript::fun(&Colour::a), "a");

		script->add(chaiscript::fun([](Colour& lhs, const Colour rhs) {return (lhs = rhs); }), "=");

	}
	Colour::Colour() : r(r), g(g), b(b), a(a) {
	}

	Colour::Colour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {
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
}