#pragma once

#include "TimingType.h"
#include <SDL2\SDL.h>

namespace Val {
	class Game;
	class RenderingEngine;

	class GameState {
	public:
		GameState(Game * const game);
		~GameState();

		virtual void initialise();

		virtual void rawEvent(const SDL_Event& e);
		virtual void update(const TimingType& deltaTime);
		virtual void render(const TimingType& deltaTime, RenderingEngine * const engine);
	protected:
		Game *const game;
	};
}