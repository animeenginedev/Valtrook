#pragma once

#include <string>

#include "TimingType.h"
#include "InputManager.h"
#include "Game.h"

namespace Val {
	class RenderingEngine;

	class Engine {
	public:
		Engine();
		~Engine();

		void start();
		void stop();
		bool isRunning() const;

		InputManager const* const getInputManager() const;

		RenderingEngine const* getRenderingEngine() const;
		void setCustomRenderer(RenderingEngine* renderer);
		void resetRendererToDefault();
		bool isRendererDefault() const;
		bool isRendererCustom() const;

		void setTargetFrameRate(unsigned int frameRate);
		void setTargetUpdateRate(unsigned int updateRate);
		void setPerformanceOutputRate(unsigned int performanceOutput);

		unsigned int getTargetFrameRate() const;
		unsigned int getTargetUpdateRate() const;
		unsigned int getPerformanceOutputRate() const;
	protected:
		bool running;

		//INPUT MANAGER MUST COME BEFORE GAME, initialization is declartion order and game relies on inputManager
		InputManager inputManager;
		Game game;
		RenderingEngine* renderer, *defaultRenderer;
		bool usingCustomRenderer;

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