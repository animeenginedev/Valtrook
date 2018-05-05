#pragma once

#include "ParticleFunction.h"
#include <vector>

namespace Val {
	template<typename Type>
	class BezierCurve : public ParticleFunction<Type> {
	public:
		BezierCurve();
		~BezierCurve();

		void UpdateValue(float deltaTime, float percentage, std::vector<Type> *Data) override;
	};

	template<typename Type>
	BezierCurve<Type>::BezierCurve() {
	}

	template<typename Type>
	BezierCurve<Type>::~BezierCurve() {
	}

	template<typename Type>
	void BezierCurve<Type>::UpdateValue(float deltaTime, float percentage, std::vector<Type> *Data) {
		std::vector<Type> DerivitivePoints = std::vector<Type>(Data->begin() + 2, Data->end());
		while (DerivitivePoints.size() > 1) {
			for (unsigned int i = 0; i < DerivitivePoints.size() - 1; i++) {
				DerivitivePoints[i] = DerivitivePoints[i] + ((DerivitivePoints[i + 1] - DerivitivePoints[i]) * percentage);
			}
			DerivitivePoints.pop_back();
		}
		(*Data)[0] -= (*Data)[1];
		(*Data)[1] = DerivitivePoints[0];
		(*Data)[0] += (*Data)[1];
	}
}