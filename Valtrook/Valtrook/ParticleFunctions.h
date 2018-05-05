#pragma once

#include "ParticleFunction.h"

#include "Colour.h"

#include "StaticValue.h"
#include "LinearMechanic.h"
#include "LinearInterp.h"
#include "BezierCurve.h"

namespace Val {
	class FunctionBuilder {
	public:
		template<typename Type>
		static std::vector<Type> BuildLinearInterp(Type Offset, Type Start, Type End) {
			return std::vector<Type>({ Offset, Type(), Start, End });
		}

		template<typename Type>
		static std::vector<Type> BuildBezierCurve(Type Offset, std::vector<Type> Points) {
			std::vector<Type> BezierCurve = std::vector<Type>();
			BezierCurve.push_back(Offset);
			BezierCurve.push_back(Type());
			for (unsigned int i = 0; i < Points.size(); ++i) {
				BezierCurve.push_back(Points[i]);
			}
			return BezierCurve;
		}
	};

	template<typename Type>
	class ParticleFunctionsType {
	public:
		ParticleFunctionsType();

		StaticValue<Type>* Function_Static;
		LinearMechanic<Type>* Function_LinearMechanic;
		LinearInterp<Type>* Function_LinearInterp;
		BezierCurve<Type>* Function_BezierCurve;
	};

	template<typename Type>
	inline ParticleFunctionsType<Type>::ParticleFunctionsType() {
		Function_Static = new StaticValue<Type>();
		Function_LinearMechanic = new LinearMechanic<Type>();
		Function_LinearInterp = new LinearInterp<Type>();
		Function_BezierCurve = new BezierCurve<Type>();
	}

#define ParticleFloats ParticleFunctions::Functions_Float
#define ParticleColours ParticleFunctions::Functions_TrueColour

	class ParticleFunctions {
		ParticleFunctions();
	public:
		static ParticleFunctionsType<float> Functions_Float;
		static ParticleFunctionsType<TrueColour> Functions_TrueColour;
	};
}