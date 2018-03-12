#pragma once

#include "GameState.h"

namespace Val {
	class Game;

	class SplashState : public GameState {
	public:
		SplashState(Game * const game) : GameState(game) {};

		void initialise() override;

		void update(const TimingType& deltaTime) override;
		void render(const TimingType& deltaTime, RenderingEngine * const engine) override;
	};
}