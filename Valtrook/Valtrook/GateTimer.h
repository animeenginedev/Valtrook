#pragma once

#include "TimingType.h"
#include "Max.h"
#include <type_traits>
#include "RegisterToScript.h"

namespace Val {
	//Countdown timer from the setLength, holds the original value so you can get a perecentage of time taken
	template<typename Real = TimingType, 
		typename = std::enable_if_t<std::is_floating_point<Real>::value>>
	class GateTimer : public RegisterToScript {
	public:
		GateTimer() {};
		~GateTimer() {};

		void registerToScript(chaiscript::ChaiScript* script) {
			script->add(chaiscript::user_type<GateTimer<float>>(), "GateTimer");

			script->add(chaiscript::constructor<GateTimer()>(), "GateTimer");

			script->add(chaiscript::fun(&GateTimer<float>::update), "update");
			script->add(chaiscript::fun(&GateTimer<float>::reset), "reset");
			script->add(chaiscript::fun(&GateTimer<float>::setLength), "setLength");
			script->add(chaiscript::fun(&GateTimer<float>::getProgress), "getProgress");

		}

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