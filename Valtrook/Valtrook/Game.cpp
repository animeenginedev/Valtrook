#include "Game.h"

#include "GameState.h"
#include "InputManager.h"
#include "RenderingEngine.h"
#include "SplashState.h"
#include "MenuState.h"
#include "RuntimeConstants.h"

namespace Val {
	Game::Game(InputManager const* const manager, AudioManager * audioManager) : inputManager(manager), audioManager(audioManager), currentState(nullptr) {
	}

	Game::~Game() {
	}

	void Game::initialise() {

		defaultCamera = new OrthographicCamera();
		((OrthographicCamera*)defaultCamera)->initialise(PixelToWorld<int, float>(RuntimeConstants::Instance->Window_Size.get().first), PixelToWorld<int, float>(RuntimeConstants::Instance->Window_Size.get().second));
		((OrthographicCamera*)defaultCamera)->update(0.0f);
		currentCamera = defaultCamera;
		
		splashState = new SplashState(this);
		menuState = new MenuState(this);

		splashState->initialise(menuState);
		menuState->initialise();

		currentState = splashState;
		splashState->onBecomeActive();
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
}