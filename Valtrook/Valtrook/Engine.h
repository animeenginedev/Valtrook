#pragma once

#include <string>

#include "TimingType.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "Game.h"

#include "RegisterToScript.h"
#include <chaiscript\chaiscript.hpp>

namespace Val {
	class RenderingEngine;

	class Engine : public RegisterToScript {
	public:
		Engine();
		~Engine();

		void start();
		void stop();
		bool isRunning() const;

		AudioManager * getAudioManager();
		InputManager const* const getInputManager() const;
		Game* getGame();
		chaiscript::ChaiScript* getScriptingEngine();

		RenderingEngine  *  getRenderingEngine() const;
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

		void registerToScript(chaiscript::ChaiScript* script);
	protected:
		bool running;
		
		chaiscript::ChaiScript scriptingEngine;

		//INPUT MANAGER MUST COME BEFORE GAME, initialization is declartion order and game relies on inputManager
		InputManager inputManager;
		AudioManager audioManager;
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