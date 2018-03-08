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

#include "Angle.h"
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
		setTargetFrameRate(240);
		setTargetUpdateRate(120);
		setPerformanceOutputRate(10);

		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->AssetPath); 

		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->ShaderPath);
		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->TexturePath);
		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->FontPath);
		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->SoundPath);
		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->SettingPath);
		
		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->TextureSheetPath);


		camera.initialise(PixelToWorld<int, float>(RuntimeConstants::Instance->Window_Size.get().first), PixelToWorld<int, float>(RuntimeConstants::Instance->Window_Size.get().second));
		camera.update(0.0f);
		camera.getMatrix();

		RenderingEngine::InitWindow();
		renderer = new RenderingEngine();
		renderer->initialise();
		renderer->currentCamera = &camera;

		test.initialise(TextureAsset::getTexture(ResourceLocation("Raven", ".png", RuntimeConstants::Instance->TexturePath), false), 0, 0, 0.0f, PixelToWorld<int, float>(150), PixelToWorld<int, float>(150), 0.0f);
	}
	void Engine::run() {
		StopWatch updateTimer, renderingTimer, secondTimer;
		updateTimer.update();
		renderingTimer.update();
		secondTimer.update();
		TimingType updateAccumlation = TimingType(), renderAccumlation = TimingType(), secondAccumlation = TimingType();
		TimingType updateDelta, renderDelta;
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
			updateDelta = updateTimer.getCurrentDeltaSecond<TimingType>();
			updateAccumlation += updateDelta;
			if (updateAccumlation >= accumlateUpdateRate) {
				updateAccumlation -= accumlateUpdateRate;
				++updateCounter;
				//Update Here


			}

			renderingTimer.update();
			renderDelta = renderingTimer.getCurrentDeltaSecond<TimingType>();
			renderAccumlation += renderDelta;
			if (renderAccumlation >= accumlateFrameRate) {
				renderAccumlation -= accumlateFrameRate;
				++renderCounter;
				//Render Here
				float deg15 = Radians<float>(35.0f * renderDelta);
				test.setRotation(test.getRotation() + deg15);

				test.sendRenderInformation(renderer);
				camera.update(renderDelta);

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