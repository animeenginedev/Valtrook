#pragma once

#include <chrono>

//High accuracy repeating time keeper
class StopWatch {
public:
	StopWatch() :
		last(std::chrono::high_resolution_clock::now()),
		current(std::chrono::high_resolution_clock::now()) {
	};

	~StopWatch() {};

	void update() {
		last = current;
		current = std::chrono::high_resolution_clock::now();
	};

	float getCurrentDeltaSecond() {
		return std::chrono::duration<float>(current - last).count();
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> last; //< last time.
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> current; //< current time
};