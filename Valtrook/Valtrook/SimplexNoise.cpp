#include "SimplexNoise.h"

namespace Val {
	static inline int32_t fastfloor(float fp) {
		int32_t i = (int32_t)fp;
		return (fp < i) ? (i - 1) : (i);
	}

	static inline int32_t fastfloor(double fp) {
		int32_t i = (int32_t)fp;
		return (fp < i) ? (i - 1) : (i);
	}

	static float grad(int32_t hash, float x, float y) {
		int32_t h = hash & 0x3F;  // Convert low 3 bits of hash code
		float u = h < 4 ? x : y;  // into 8 simple gradient directions,
		float v = h < 4 ? y : x;  // and compute the dot product with (x,y).
		return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f*v : 2.0f*v);
	}

	static double grad(int32_t hash, double x, double y) {
		int32_t h = hash & 0x3F;  // Convert low 3 bits of hash code
		double u = h < 4 ? x : y;  // into 8 simple gradient directions,
		double v = h < 4 ? y : x;  // and compute the dot product with (x,y).
		return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f*v : 2.0f*v);
	}

	SimplexNoise::SimplexNoise(long seed, float frequency, float amplitude, float lacunarity, float persistence) : Noise(seed, frequency, amplitude, lacunarity, persistence) {
		srand(seed);
		std::fill_n(perm, 256, 0);
		for (int i = 0; i < 255; i++) {
			int placement = rand() % 255;
			while (true) {
				if (perm[placement] == 0) {
					perm[placement] = i;
					break;
				}
				placement++;
				placement = placement % 255;
			}
		}
	}

	SimplexNoise::~SimplexNoise() {
	}

	float SimplexNoise::generateNoise(float x, float y) {
		x += seed;
		y += seed;
		float n0, n1, n2;   // Noise contributions from the three corners

							// Skewing/Unskewing factors for 2D
		const float F2 = 0.366025403f;  // F2 = (sqrt(3) - 1) / 2
		const float G2 = 0.211324865f;  // G2 = (3 - sqrt(3)) / 6   = F2 / (1 + 2 * K)

										// Skew the input space to determine which simplex cell we're in
		float s = (x + y) * F2;  // Hairy factor for 2D
		float xs = x + s;
		float ys = y + s;
		int32_t i = fastfloor(xs);
		int32_t j = fastfloor(ys);

		// Unskew the cell origin back to (x,y) space
		float t = static_cast<float>(i + j) * G2;
		float X0 = i - t;
		float Y0 = j - t;
		float x0 = x - X0;  // The x,y distances from the cell origin
		float y0 = y - Y0;

		// For the 2D case, the simplex shape is an equilateral triangle.
		// Determine which simplex we are in.
		int32_t i1, j1;  // Offsets for second (middle) corner of simplex in (i,j) coords
		if (x0 > y0) {   // lower triangle, XY order: (0,0)->(1,0)->(1,1)
			i1 = 1;
			j1 = 0;
		} else {   // upper triangle, YX order: (0,0)->(0,1)->(1,1)
			i1 = 0;
			j1 = 1;
		}

		// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
		// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
		// c = (3-sqrt(3))/6

		float x1 = x0 - i1 + G2;            // Offsets for middle corner in (x,y) unskewed coords
		float y1 = y0 - j1 + G2;
		float x2 = x0 - 1.0f + 2.0f * G2;   // Offsets for last corner in (x,y) unskewed coords
		float y2 = y0 - 1.0f + 2.0f * G2;

		// Calculate the contribution from the first corner
		float t0 = 0.5f - x0*x0 - y0*y0;
		if (t0 < 0.0f) {
			n0 = 0.0f;
		} else {
			t0 *= t0;
			n0 = t0 * t0 * grad(hash(i + hash(j)), x0, y0);
		}

		// Calculate the contribution from the second corner
		float t1 = 0.5f - x1*x1 - y1*y1;
		if (t1 < 0.0f) {
			n1 = 0.0f;
		} else {
			t1 *= t1;
			n1 = t1 * t1 * grad(hash(i + i1 + hash(j + j1)), x1, y1);
		}

		// Calculate the contribution from the third corner
		float t2 = 0.5f - x2*x2 - y2*y2;
		if (t2 < 0.0f) {
			n2 = 0.0f;
		} else {
			t2 *= t2;
			n2 = t2 * t2 * grad(hash(i + 1 + hash(j + 1)), x2, y2);
		}

		// Add contributions from each corner to get the final noise value.
		// The result is scaled to return values in the interval [-1,1].
		return 45.23065f * (n0 + n1 + n2);
	}

	double SimplexNoise::generateNoise(double x, double y) {
		x += seed;
		y += seed;
		double n0, n1, n2;   // Noise contributions from the three corners

							 // Skewing/Unskewing factors for 2D
		const double F2 = 0.366025403;  // F2 = (sqrt(3) - 1) / 2
		const double G2 = 0.211324865;  // G2 = (3 - sqrt(3)) / 6   = F2 / (1 + 2 * K)

										// Skew the input space to determine which simplex cell we're in
		double s = (x + y) * F2;  // Hairy factor for 2D
		double xs = x + s;
		double ys = y + s;
		int32_t i = fastfloor(xs);
		int32_t j = fastfloor(ys);

		// Unskew the cell origin back to (x,y) space
		double t = static_cast<double>(i + j) * G2;
		double X0 = i - t;
		double Y0 = j - t;
		double x0 = x - X0;  // The x,y distances from the cell origin
		double y0 = y - Y0;

		// For the 2D case, the simplex shape is an equilateral triangle.
		// Determine which simplex we are in.
		int32_t i1, j1;  // Offsets for second (middle) corner of simplex in (i,j) coords
		if (x0 > y0) {   // lower triangle, XY order: (0,0)->(1,0)->(1,1)
			i1 = 1;
			j1 = 0;
		} else {   // upper triangle, YX order: (0,0)->(0,1)->(1,1)
			i1 = 0;
			j1 = 1;
		}

		// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
		// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
		// c = (3-sqrt(3))/6

		double x1 = x0 - i1 + G2;            // Offsets for middle corner in (x,y) unskewed coords
		double y1 = y0 - j1 + G2;
		double x2 = x0 - 1.0 + 2.0 * G2;   // Offsets for last corner in (x,y) unskewed coords
		double y2 = y0 - 1.0 + 2.0 * G2;

		// Calculate the contribution from the first corner
		double t0 = 0.5 - x0*x0 - y0*y0;
		if (t0 < 0.0) {
			n0 = 0.0;
		} else {
			t0 *= t0;
			n0 = t0 * t0 * grad(hash(i + hash(j)), x0, y0);
		}

		// Calculate the contribution from the second corner
		double t1 = 0.5 - x1*x1 - y1*y1;
		if (t1 < 0.0) {
			n1 = 0.0;
		} else {
			t1 *= t1;
			n1 = t1 * t1 * grad(hash(i + i1 + hash(j + j1)), x1, y1);
		}

		// Calculate the contribution from the third corner
		double t2 = 0.5 - x2*x2 - y2*y2;
		if (t2 < 0.0) {
			n2 = 0.0;
		} else {
			t2 *= t2;
			n2 = t2 * t2 * grad(hash(i + 1 + hash(j + 1)), x2, y2);
		}

		// Add contributions from each corner to get the final noise value.
		// The result is scaled to return values in the interval [-1,1].
		return 45.23065 * (n0 + n1 + n2);
	}
}