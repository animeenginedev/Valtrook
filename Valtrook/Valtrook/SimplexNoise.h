#pragma once
#include "Noise.h"

namespace Val {
	//The implementation of the simplex noise algorithim is basically copied from Stefan Gustavson java code
	class SimplexNoise : public Noise {
	public:
		SimplexNoise(long seed, float frequency = 1.0f, float amplitude = 1.0f, float lacunarity = 2.0f, float persistence = (1 / 2.0f));
		~SimplexNoise();

		virtual float generateNoise(float x, float y);
		virtual double generateNoise(double x, double y);
	protected:
		uint8_t perm[256];

		inline uint8_t hash(int32_t i) {
			return perm[static_cast<uint8_t>(i)];
		}
	};

}