#include "Engine.h"

#include "StopWatch.h"
#include "Logger.h"
#include "RenderingEngine.h"
#include "RuntimeConstants.h"
#include "Texture.h"
#include "TextureAsset.h"
#include "TaskMaster.h"
#include "BindingRegister.h"


#include <sdl_ttf.h>
#include <thread>
#include <chrono>

#include <windows.h> //For CreateDirectory.
static void CreateDirectoryIfItDoesNotExist(std::string directory) {
	CreateDirectory(directory.c_str(), NULL);
}

#include "Angle.h"
namespace Val {
	Engine::Engine() : running(false), scriptingEngine(), inputManager(), game(&inputManager, &audioManager, &scriptingEngine) {
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

	AudioManager * Engine::getAudioManager() {
		return &audioManager;
	}

	InputManager const * const Engine::getInputManager() const {
		return &inputManager;
	}

	Game * Engine::getGame() {
		return &game;
	}

	chaiscript::ChaiScript * Engine::getScriptingEngine() {
		return &scriptingEngine;
	}

	RenderingEngine * Engine::getRenderingEngine() const {
		return renderer;
	}

	void Engine::setCustomRenderer(RenderingEngine * renderer) {
		this->renderer = renderer;
		usingCustomRenderer = true;
	}

	void Engine::resetRendererToDefault() {
		renderer = defaultRenderer;
		usingCustomRenderer = false;
	}

	bool Engine::isRendererDefault() const {
		return !usingCustomRenderer;
	}

	bool Engine::isRendererCustom() const {
		return usingCustomRenderer;
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

	void Engine::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<Engine>(), "Engine");

		script->add(chaiscript::fun(&Engine::stop), "stop");
		script->add(chaiscript::fun(&Engine::isRunning), "isRunning");

		script->add(chaiscript::fun(&Engine::getAudioManager), "getAudioManager");
		script->add(chaiscript::fun(&Engine::getInputManager), "getInputManager");
		script->add(chaiscript::fun(&Engine::getRenderingEngine), "getRenderingEngine");

		script->add(chaiscript::fun(&Engine::setTargetFrameRate), "setTargetFrameRate");
		script->add(chaiscript::fun(&Engine::setTargetUpdateRate), "setTargetUpdateRate");
		script->add(chaiscript::fun(&Engine::setPerformanceOutputRate), "setPerformanceOutputRate");

		script->add(chaiscript::fun(&Engine::getTargetFrameRate), "getTargetFrameRate");
		script->add(chaiscript::fun(&Engine::getTargetUpdateRate), "getTargetUpdateRate");
		script->add(chaiscript::fun(&Engine::getPerformanceOutputRate), "getPerformanceOutputRate");
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
		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->ScriptingPath); 
		
		CreateDirectoryIfItDoesNotExist(RuntimeConstants::Instance->TextureSheetPath);

		RenderingEngine::InitWindow();
		usingCustomRenderer = false;
		defaultRenderer = new RenderingEngine();
		defaultRenderer->initialise();
		renderer = defaultRenderer;

		audioManager.initialise();
		
		if (TTF_Init() == -1) {
			Logger::Instance->logMessage(LogLevel::SEVERE, "Failed to initialise SDL_TTF");
		}

		TaskMaster::initialise(std::thread::hardware_concurrency());

		Texture::errorTexture = TextureAsset::getTexture("errorTexture");

		BindingRegister::RegisterBindings(this, scriptingEngine);

		game.initialise();
	}
	void Engine::run() {
		StopWatch updateTimer, renderingTimer, secondTimer;
		updateTimer.update();
		renderingTimer.update();
		secondTimer.update();
		TimingType updateAccumlation = TimingType(), renderAccumlation = TimingType(), secondAccumlation = TimingType();
		TimingType updateDelta, renderDelta;
		unsigned int secondCounter = 0u, updateCounter = 0u, renderCounter = 0u, loopCounter = 0u, performanceCounter = performanceOutputRate;
		SDL_Event e;
		while (running) {

			//Events
			//A note on inputs; if you manage to hit a key up and down faster than it takes to run an update, then you'll have a keypressed thats missed, but good luck pressing a key down faster then 1/120th of a second.
			if (SDL_PollEvent(&e) == 1) {
				do {
					switch (e.type) {
						case SDL_QUIT:
							running = false;
						case SDL_KEYDOWN:
							inputManager.updateKeyState(e.key.keysym.sym, true);
							break;
						case SDL_KEYUP:
							inputManager.updateKeyState(e.key.keysym.sym, false);
							break;
						case SDL_MOUSEBUTTONDOWN:
							inputManager.updateMouseButton(e.button.button, true);
							break;
						case SDL_MOUSEBUTTONUP:
							inputManager.updateMouseButton(e.button.button, false);
							break;
						case SDL_MOUSEWHEEL:
							inputManager.updateMouseWheel(e.wheel.y);
							break;
						default:
							break;
					}
					game.rawEvent(e);
				} while (SDL_PollEvent(&e) == 1);
			}


			updateTimer.update();
			updateDelta = updateTimer.getCurrentDeltaSecond<TimingType>();
			updateAccumlation += updateDelta;
			if (updateAccumlation >= accumlateUpdateRate) {
				updateAccumlation -= accumlateUpdateRate;
				++updateCounter;

				if (updateAccumlation > accumlateUpdateRate) {
					game.update(updateAccumlation + accumlateUpdateRate);
					updateAccumlation = 0.0f;
				} else {
					game.update(accumlateUpdateRate);
				}

				inputManager.update();
			}

			renderingTimer.update();
			renderDelta = renderingTimer.getCurrentDeltaSecond<TimingType>();
			renderAccumlation += renderDelta;
			if (renderAccumlation >= accumlateFrameRate) {
				renderAccumlation -= accumlateFrameRate;
				++renderCounter;


				if (renderAccumlation > accumlateFrameRate) {
					game.render(renderAccumlation +accumlateFrameRate, renderer);
					renderAccumlation = 0.0f;
				} else {
					game.render(accumlateFrameRate, renderer);
				}
				
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
					Logger::Instance->logMessage(LogLevel::INFO, "LR: " + std::to_string(static_cast<float>(loopCounter) / 10.0f));

					updateCounter = 0;
					renderCounter = 0;
					loopCounter = 0;
				}

				//Output Log
				Logger::Instance->outputLog();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			++loopCounter;
		}

		TaskMaster::shutdown();
	}
}