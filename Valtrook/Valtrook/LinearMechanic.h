#pragma once

#include "ParticleFunction.h"

#include <vector>

namespace Val {
	template<typename Type>
	class LinearMechanic : public ParticleFunction<Type> {
	public:
		LinearMechanic();
		~LinearMechanic();

		void UpdateValue(float deltaTime, float percentage, std::vector<Type>* Data) override;
	};

	template<typename Type>
	inline LinearMechanic<Type>::LinearMechanic() {
	}

	template<typename Type>
	inline LinearMechanic<Type>::~LinearMechanic() {
	}

	template<typename Type>
	inline void LinearMechanic<Type>::UpdateValue(float deltaTime, float percentage, std::vector<Type>* Data) {
		for (int i = Data->size() - 2; i != -1; --i) {
			(*Data)[i] += (*Data)[i + 1] * deltaTime;
		}
	}
}