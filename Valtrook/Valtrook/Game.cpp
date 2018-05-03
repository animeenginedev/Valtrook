#include "Game.h"

#include "GameState.h"
#include "InputManager.h"
#include "RenderingEngine.h"
#include "SplashState.h"
#include "MenuState.h"
#include "RuntimeConstants.h"

namespace Val {
	Game::Game(InputManager const* const manager, AudioManager * audioManager, chaiscript::ChaiScript* scriptingEngine) : inputManager(manager), audioManager(audioManager), scriptingEngine(scriptingEngine), currentState(nullptr), entryPointScript("game.chai", scriptingEngine) {
	}

	Game::~Game() {
	}

	void Game::initialise() {

		defaultCamera = new OrthographicCamera();
		((OrthographicCamera*)defaultCamera)->initialise(PixelToWorld<int, float>(RuntimeConstants::Instance->Window_Size.get().first), PixelToWorld<int, float>(RuntimeConstants::Instance->Window_Size.get().second));
		((OrthographicCamera*)defaultCamera)->update(0.0f);
		currentCamera = defaultCamera;
		
		entryPointScript.reload();
		entryPointScript.safetlyRunFunc(entryPointScript.getFunction<std::function<void()>> ("initialise"));

		if (currentState == nullptr) {
			if (nextState != nullptr) {
				if (callbackOnStateChange.operator bool())
					callbackOnStateChange();
				currentState = nextState;
				nextState = nullptr;

				currentState->onBecomeActive();
			}
		}
	}

	void Game::rawEvent(const SDL_Event & e) {
		currentState->rawEvent(e);
	}

	void Game::update(const TimingType & deltaTime) {
		currentState->update(deltaTime);

		if (nextState != nullptr) {
			currentState->onBecomeInactive();
			if (callbackOnStateChange.operator bool())
				callbackOnStateChange();
			currentState = nextState;
			nextState = nullptr;

			currentState->onBecomeActive();
		}
	}

	void Game::render(const TimingType & deltaTime, RenderingEngine * const engine) {
		currentState->render(deltaTime, engine);
		
		currentCamera->update(deltaTime);
		engine->shaderObject->setCamera(currentCamera);
	}

	InputManager const * const Game::getInputManager() const {
		return inputManager;
	}

	AudioManager * Game::getAudioManager() const {
		return audioManager;
	}

	chaiscript::ChaiScript * Game::getScriptingEngine() {
		return scriptingEngine;
	}

	GameState * Game::getState() const {
		return currentState;
	}

	void Game::setState(GameState * state, std::function<void()> Callback) {
		if (state == nullptr)
			return;
		nextState = state;
		callbackOnStateChange = Callback;
	}
	bool Game::hasNextState() const {
		return nextState != nullptr;
	}
	void Game::setCamera(Camera * Camera) {
		currentCamera = Camera;
	}
	Camera * Game::getCamera() {
		return currentCamera;
	}
	void Game::resetToDefaultCamera() {
		currentCamera = defaultCamera;
	}
	GameState * Game::getNamedState(const std::string name) const {
		return namedStates.find(name) == namedStates.end() ? nullptr : namedStates.at(name);
	}
	void Game::addNamedState(std::string scriptName, std::string name) {
		namedStates.insert(std::make_pair(name, new GameState(this, scriptName)));
	}
	void Game::setNamedState(const std::string name) {
		auto state = getNamedState(name);
		if (state != nullptr)
			setState(state);
	}
	void Game::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<Game>(), "Game");
		script->add_global(chaiscript::var(this), "Game");
		
		script->add(chaiscript::fun(&Game::getNamedState), "getNamedState");
		script->add(chaiscript::fun(&Game::addNamedState), "addNamedState");
		script->add(chaiscript::fun(&Game::setNamedState), "setNamedState");

	}
}