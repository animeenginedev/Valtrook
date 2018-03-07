#include "Engine.h"

#include "StopWatch.h"
#include "Logger.h"
#include "RenderingEngine.h"
#include "RuntimeConstants.h"

#include <thread>
#include <chrono>

#include <windows.h> //For CreateDirectory.
static void CreateDirectoryIfItDoesNotExist(std::string directory) {
	CreateDirectory(directory.c_str(), NULL);
}

namespace Val {
	Engine::Engine() : running(false) {

	}

	Engine::~Engine() {
	}

	void Engine::start() {
		//We don't want some retard to call start from inside its own function just because it's dumb.
		if (running == false) {
			running = true;
			initialise();
			run();
		}
	}

	void Engine::stop() {
		running = false;
	}

	bool Engine::isRunning() const {
		return running;
	}

	void Engine::setTargetFrameRate(unsigned int frameRate) {
		targetFrameRate = frameRate;
		accumlateFrameRate = static_cast<TimingType>(1.0) / static_cast<TimingType>(frameRate);
		renderExpectedString = " (" + std::to_string(frameRate) + ")";
	}

	void Engine::setTargetUpdateRate(unsigned int updateRate) {
		targetUpdateRate = updateRate;
		accumlateUpdateRate = static_cast<TimingType>(1.0) / static_cast<TimingType>(updateRate);
		updateExpectedString = " (" + std::to_string(targetUpdateRate) + ")"; 
	}

	void Engine::setPerformanceOutputRate(unsigned int performanceOutput) {
		performanceOutputRate = performanceOutput;
		performanceAverageString = "Avg Over " + std::to_string(performanceOutput) + "s";
	}

	unsigned int Engine::getTargetFrameRate() const {
		return targetFrameRate;
	}

	unsigned int Engine::getTargetUpdateRate() const {
		return targetUpdateRate;
	}

	unsigned int Engine::getPerformanceOutputRate() const {
		return performanceOutputRate;
	}

	void Engine::initialise() {
		setTargetFrameRate(120);
		setTargetUpdateRate(60);
		setPerformanceOutputRate(10);

		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->AssetPath); 

		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->ShaderPath);
		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->TexturePath);
		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->FontPath);
		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->SoundPath);
		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->SettingPath);
		
		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->TextureSheetPath);


		RenderingEngine::InitWindow();
		renderer = new RenderingEngine();
		renderer->initialise();
	}

	void Engine::run() {
		StopWatch updateTimer, renderingTimer, secondTimer;
		updateTimer.update();
		renderingTimer.update();
		secondTimer.update();
		TimingType updateAccumlation = TimingType(), renderAccumlation = TimingType(), secondAccumlation = TimingType();
		unsigned int secondCounter = 0u, updateCounter = 0u, renderCounter = 0u, performanceCounter = performanceOutputRate;
		SDL_Event e;
		while (running) {

			//Events
			if (SDL_PollEvent(&e) == 1) {
				do {
					switch (e.type) {
						case SDL_QUIT:
							running = false;
						default:
							break;
					}
				} while (SDL_PollEvent(&e) == 1);
			}

			updateTimer.update();
			updateAccumlation += updateTimer.getCurrentDeltaSecond<TimingType>();
			if (updateAccumlation >= accumlateUpdateRate) {
				updateAccumlation -= accumlateUpdateRate;
				++updateCounter;
				//Update Here


			}

			renderingTimer.update();
			renderAccumlation += renderingTimer.getCurrentDeltaSecond<TimingType>();
			if (renderAccumlation >= accumlateFrameRate) {
				renderAccumlation -= accumlateFrameRate;
				++renderCounter;
				//Render Here

				renderer->render();
			}

			secondTimer.update();
			secondAccumlation += secondTimer.getCurrentDeltaSecond<TimingType>();
			if (secondAccumlation >= static_cast<TimingType>(1.0)) {
				++secondCounter;
				--performanceCounter;
				secondAccumlation -= static_cast<TimingType>(1.0);

				//Output Performance
				if (performanceCounter == 0) {
					performanceCounter = performanceOutputRate;
					Logger::Instance->logMessage(LogLevel::INFO, performanceAverageString);
					Logger::Instance->logMessage(LogLevel::INFO, "U: " + std::to_string(static_cast<float>(updateCounter) / 10.0f) + updateExpectedString);
					Logger::Instance->logMessage(LogLevel::INFO, "R: " + std::to_string(static_cast<float>(renderCounter) / 10.0f) + renderExpectedString);

					updateCounter = 0;
					renderCounter = 0;
				}

				//Output Log
				Logger::Instance->outputLog();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}