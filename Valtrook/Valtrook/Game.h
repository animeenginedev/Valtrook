#pragma once

#include "OrthographicCamera.h"
#include "TimingType.h"

#include <functional>
#include <SDL2\SDL.h>

namespace Val {
	class GameState;
	class RenderingEngine;
	class InputManager;

	class SplashState;

	class Game {
	public:
		Game(InputManager const* const manager);
		~Game();

		void initialise();

		void rawEvent(const SDL_Event& e);
		void update(const TimingType& deltaTime);
		void render(const TimingType& deltaTime, RenderingEngine * const engine);

		InputManager const* const getInputManager() const;

		GameState* getState() const;
		//happens at the end of the next update(), callback is set to null when it's completed.
		void setState(GameState* state, std::function<void()> Callback = nullptr);

		GameState* splashState;
	protected:
		GameState* currentState;
		GameState* nextState;
		std::function<void()> callbackOnStateChange;
		OrthographicCamera* currentCamera;
		InputManager const* const inputManager;
	};

}