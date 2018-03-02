#pragma once

namespace Val {
	class Engine {
		typedef float TimingType;
	public:
		Engine();
		~Engine();

		void start();
		void stop();
		bool isRunning() const;

		void setTargetFrameRate(unsigned int frameRate);
		void setTargetUpdateRate(unsigned int updateRate);

		unsigned int getTargetFrameRate() const;
		unsigned int getTargetUpdateRate() const;
	protected:
		bool running;

		//Raw numbers of how many times you want to update
		unsigned int targetFrameRate, targetUpdateRate;
		//Actual time that passes for an update (1 / target)
		TimingType accumlateFrameRate, accumlateUpdateRate;

		void initialise();
		void run();
	};

}