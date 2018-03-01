#include "Noise.h"

namespace Val {
	Noise::~Noise() {
	}

	bool Noise::generateNoise(float xIn, float y, unsigned int width, std::vector<float>* data) {
		if (data == nullptr)
			return false;

		unsigned int c = 0, cMax = data->size(), h = 0;
		while (c != cMax) {
			for (unsigned int x = 0; x < width; x++) {
				(*data)[c++] = generateNoise(x + xIn, y + h);
				if (c == cMax)
					return true;
			}
			h++;
		}

		return true;
	}

	bool Noise::generateNoise(double xIn, double y, unsigned int width, std::vector<double>* data) {
		if (data == nullptr)
			return false;

		unsigned int c = 0, cMax = data->size(), h = 0;
		while (c != cMax) {
			for (unsigned int x = 0; x < width; x++) {
				(*data)[c++] = generateNoise(x + xIn, y + h);
				if (c == cMax)
					return true;
			}
			h++;
		}

		return true;
	}

	float Noise::generateOctaveNoise(float x, float y, unsigned int octaves) {
		float output = 0.f;
		float denom = 0.f;
		float frequency = this->frequency;
		float amplitude = this->amplitude;

		for (unsigned int i = 0; i < octaves; i++) {
			output += (amplitude * generateNoise(x * frequency, y * frequency));
			denom += amplitude;

			frequency *= lacunarity;
			amplitude *= persistence;
		}

		return (output / denom);
	}

	double Noise::generateOctaveNoise(double x, double y, unsigned int octaves) {
		float output = 0.f;
		float denom = 0.f;
		float frequency = this->frequency;
		float amplitude = this->amplitude;

		for (unsigned int i = 0; i < octaves; i++) {
			output += static_cast<float>(static_cast<double>(amplitude) * generateNoise(x * frequency, y * frequency));
			denom += amplitude;

			frequency *= lacunarity;
			amplitude *= persistence;
		}

		return (output / denom);
	}

	bool Noise::generateOctaveNoise(float xIn, float y, unsigned int octaves, unsigned int width, std::vector<float>* data) {
		if (data == nullptr)
			return false;

		unsigned int c = 0, cMax = data->size(), h = 0;
		while (c != cMax) {
			for (unsigned int x = 0; x < width; x++) {
				(*data)[c++] = generateOctaveNoise(x + xIn, y + h, octaves);
				if (c == cMax)
					return true;
			}
			h++;
		}

		return true;
	}

	bool Noise::generateOctaveNoise(double xIn, double y, unsigned int octaves, unsigned int width, std::vector<double>* data) {
		if (data == nullptr)
			return false;

		unsigned int c = 0, cMax = data->size(), h = 0;
		while (c != cMax) {
			for (unsigned int x = 0; x < width; x++) {
				(*data)[c++] = generateOctaveNoise(x + xIn, y + h, octaves);
				if (c == cMax)
					return true;
			}
			h++;
		}

		return true;
	}

	Noise::Noise(long seed, float frequency, float amplitude, float lacunarity, float persistence) : seed(seed), frequency(frequency), amplitude(amplitude), lacunarity(lacunarity), persistence(persistence) {
	}
}