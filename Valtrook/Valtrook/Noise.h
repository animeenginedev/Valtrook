#pragma once

#include <vector>
#include <array>

namespace Val {
	class Noise {
	public:
		~Noise();

		virtual float generateNoise(float x, float y) = 0;
		virtual double generateNoise(double x, double y) = 0;

		//Return false if a bad vector was passed for data, the vector must be pre resized.
		virtual bool generateNoise(float x, float y, unsigned int width, std::vector<float>* data);
		//Return false if a bad vector was passed for data, the vector must be pre resized.
		virtual bool generateNoise(double x, double y, unsigned int width, std::vector<double>* data);

		virtual float generateOctaveNoise(float x, float y, unsigned int octaves);
		virtual double generateOctaveNoise(double x, double y, unsigned int octaves);

		//Return false if a bad vector was passed for data, the vector must be pre resized.
		virtual	bool generateOctaveNoise(float x, float y, unsigned int octaves, unsigned int width, std::vector<float>* data);
		//Return false if a bad vector was passed for data, the vector must be pre resized.
		virtual bool generateOctaveNoise(double x, double y, unsigned int octaves, unsigned int width, std::vector<double>* data);

		inline std::vector<float>* getVectorF(unsigned int width, unsigned int height) { return new std::vector<float>(width*height); }
		inline std::vector<double>* getVectorD(unsigned int width, unsigned int height) { return new std::vector<double>(width*height); }
	protected:
		//persistance being 1 / lacunarity by default)
		Noise(long seed, float frequency = 1.0f, float amplitude = 1.0f, float lacunarity = 2.0f, float persistence = (1 / 2.0f));

		long seed;

		float frequency;     //Width  
		float amplitude;     //Height 
		float lacunarity;    //Frequency Multiplier between octaves
		float persistence;   //Loss of amplitude between octaves (normally 1/lacunarity)
	};
}