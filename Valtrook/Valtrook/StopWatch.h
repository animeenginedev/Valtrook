#pragma once

#include <chrono>
#include <type_traits>

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

	template<typename Real, typename = std::enable_if_t<std::is_floating_point<Real>::value>>
	Real getCurrentDeltaSecond() {
		return std::chrono::duration<Real>(current - last).count();
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> last; //< last time.
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> current; //< current time
};