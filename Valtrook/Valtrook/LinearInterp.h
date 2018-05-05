#pragma once

#include "ParticleFunction.h"

#include <vector>

namespace Val {
	template<typename Type>
	class LinearInterp : public ParticleFunction<Type> {
	public:
		LinearInterp();
		~LinearInterp();

		void UpdateValue(float deltaTime, float percentage, std::vector<Type> *Data) override;
	};

	template<typename Type>
	inline LinearInterp<Type>::LinearInterp() {
	}

	template<typename Type>
	inline LinearInterp<Type>::~LinearInterp() {
	}

	template<typename Type>
	inline void LinearInterp<Type>::UpdateValue(float deltaTime, float percentage, std::vector<Type> *Data) {
		(*Data)[0] -= (*Data)[1];
		(*Data)[1] = (*Data)[2] + (((*Data)[3] - (*Data)[2]) * percentage);
		(*Data)[0] += (*Data)[1];
	}
}