#include "Game.h"

#include "GameState.h"
#include "InputManager.h"
#include "RenderingEngine.h"
#include "SplashState.h"

namespace Val {
	Game::Game(InputManager const* const manager) : inputManager(manager), currentState(reinterpret_cast<GameState*>(37)) {
	}

	Game::~Game() {
	}

	void Game::initialise() {
		splashState = new SplashState(this);


		currentState = splashState;
	}

	void Game::rawEvent(const SDL_Event & e) {
		currentState->rawEvent(e);
	}

	void Game::update(const TimingType & deltaTime) {
		currentState->update(deltaTime);

		if (nextState != nullptr) {
			if (callbackOnStateChange.operator bool())
				callbackOnStateChange();
			currentState = nextState;
			nextState = nullptr;
		}
	}

	void Game::render(const TimingType & deltaTime, RenderingEngine * const engine) {
		currentState->render(deltaTime, engine);
	}

	InputManager const * const Game::getInputManager() const {
		return inputManager;
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
}