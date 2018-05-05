#pragma once

#include "ParticleFunction.h"

#include <vector>

namespace Val {
	template<typename Type>
	class StaticValue : public ParticleFunction<Type> {
	public:
		StaticValue();
		~StaticValue();

		void UpdateValue(float deltaTime, float percentage, std::vector<Type> *Data) override;
	};

	template<typename Type>
	inline StaticValue<Type>::StaticValue() {
	}

	template<typename Type>
	inline StaticValue<Type>::~StaticValue() {
	}

	template<typename Type>
	inline void StaticValue<Type>::UpdateValue(float deltaTime, float percentage, std::vector<Type> *Data) {
	}
}