#pragma once

#include "TimingType.h"
#include "Max.h"
#include <type_traits>

namespace Val {
	//Countdown timer from the setLength, holds the original value so you can get a perecentage of time taken
	template<typename Real = TimingType, 
		typename = std::enable_if_t<std::is_floating_point<Real>::value>>
	class GateTimer {
	public:
		GateTimer() {};
		~GateTimer() {};
		
		//Returns true when the timer is finished
		bool update(Real delta) {
			time = Max<Real>(time - delta, static_cast<Real>(0.0));
			return time <= static_cast<Real>(0.0);
		};

		void reset() {
			time = timeMax;
		};

		void setLength(Real t) {
			timeMax = t;
			time = t;
		};

		Real getProgress() {
			return static_cast<Real>(1.0) - (time / timeMax);
		}

		Real time;
		Real timeMax;
	};
};