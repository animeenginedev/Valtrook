#pragma once

#include <random>

namespace Val {

	class Random {
	public:
		Random();
		Random(long Seed);
		~Random();

		static Random *Instance;
		
		void setSeed(long Seed);

		int randomInt();
		int randomInt(const int &min, const int &max);
		unsigned int randomUInt();
		unsigned int randomUInt(const unsigned int &min, const unsigned int &max);
		long randomLong();
		long randomLong(const long &min, const long &max);
		float randomFloat();
		float randomFloat(const float &min, const float &max);
		double randomDouble();
		double randomDouble(const double &min, const double &max);

		std::mt19937& getGenerator();
		long getSeed();
	private:
		std::mt19937 generator;
		long seed;
	};

}