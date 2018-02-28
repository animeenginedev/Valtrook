#include "Random.h"

#include <limits>

Val::Random* Val::Random::Instance = new Random();

Val::Random::Random() {
	generator = std::default_random_engine();
	generator.seed(0);
	seed = 0;
}

Val::Random::Random(long Seed) {
	generator = std::default_random_engine();
	generator.seed(Seed);
	seed = Seed;
}

Val::Random::~Random() {
}

void Val::Random::setSeed(long Seed) {
	seed = Seed;
	generator.seed(Seed);
}

int Val::Random::randomInt() {
	return std::uniform_int<int>(std::numeric_limits<int>::min(), std::numeric_limits<int>::max())(generator);
}

int Val::Random::randomInt(const int & min, const int & max) {
	return std::uniform_int<int>(min, max)(generator);
}

unsigned int Val::Random::randomUInt() {
	return std::uniform_int<unsigned int>(std::numeric_limits<unsigned int>::min(), std::numeric_limits<unsigned int>::max())(generator);
}

unsigned int Val::Random::randomUInt(const unsigned int & min, const unsigned int & max) {
	return std::uniform_int<unsigned int>(min, max)(generator);
}

long Val::Random::randomLong() {
	return std::uniform_int<long>(std::numeric_limits<long>::min(), std::numeric_limits<long>::max())(generator);
}

long Val::Random::randomLong(const long & min, const long & max) {
	return std::uniform_int<long>(min, max)(generator);
}

float Val::Random::randomFloat() {
	return std::uniform_real<float>(std::numeric_limits<float>::min(), std::numeric_limits<float>::max())(generator);
}

float Val::Random::randomFloat(const float & min, const float & max) {
	return std::uniform_real<float>(min, max)(generator);
}

double Val::Random::randomDouble() {
	return std::uniform_real<double>(std::numeric_limits<double>::min(), std::numeric_limits<double>::max())(generator);
}

double Val::Random::randomDouble(const double & min, const double & max) {
	return std::uniform_real<double>(min, max)(generator);
}

std::mt19937 & Val::Random::getGenerator() {
	return generator;
}

long Val::Random::getSeed() {
	return seed;
}
