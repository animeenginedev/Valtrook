#pragma once

namespace Val {
	//Countdown timer from the setLength, holds the original value so you can get a perecentage of time taken
	class GateTimer {
	public:
		GateTimer();
		~GateTimer();

		//Returns true when the timer is finished
		bool update(float delta);

		void reset();
		void setLength(float t);
		float getProgress();

		float time;
		float timeMax;
	};
}