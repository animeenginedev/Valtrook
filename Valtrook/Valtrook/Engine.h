#pragma once

#include <string>

namespace Val {
	class RenderingEngine;

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
		void setPerformanceOutputRate(unsigned int performanceOutput);

		unsigned int getTargetFrameRate() const;
		unsigned int getTargetUpdateRate() const;
		unsigned int getPerformanceOutputRate() const;
	protected:
		bool running;

		RenderingEngine* renderer;

		//Raw numbers of how many times you want to update
		unsigned int targetFrameRate, targetUpdateRate, performanceOutputRate;
		//Actual time that passes for an update (1 / target)
		TimingType accumlateFrameRate, accumlateUpdateRate;

		std::string performanceAverageString;
		std::string updateExpectedString, renderExpectedString;


		void initialise();
		void run();
	};

}