#pragma once

#include "GameState.h"

#include "Rectangle.h"
#include "TimingType.h"
#include "GateTimer.h"

namespace Val {
	class Game;

	class SplashState : public GameState {
	public:
		SplashState(Game * const game) : GameState(game) {};

		void initialise(GameState* menuState);

		void update(const TimingType& deltaTime) override;
		void render(const TimingType& deltaTime, RenderingEngine * const engine) override;

	protected:
		Rectangle SplashDisplay;
		GateTimer<> splashScreenPersistance;
		GameState* menuState;

		void initialise() override;
	};
}