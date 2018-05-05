#pragma once

#include <vector>

namespace Val {
	template<typename Type>
	class ParticleFunction {
	public:
		virtual void UpdateValue(float deltaTime, float percentage, std::vector<Type> *Data) = 0;
	};
}