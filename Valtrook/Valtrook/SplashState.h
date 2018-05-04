#pragma once

#include "GameState.h"

#include "Rectangle.h"
#include "TextRectangle.h"
#include "TimingType.h"
#include "GateTimer.h"
#include "AudioManager.h"
#include <memory>

namespace Val {
	class Game;

	class SplashState : public GameState {
	public:
		SplashState(Game * const game);

		void initialise(GameState* menuState);

		void onBecomeActive() override;
		void onBecomeInactive() override;

		void update(const TimingType& deltaTime) override;
		void render(const TimingType& deltaTime, RenderingEngine * const engine) override;

	protected:
		TextRectangle SplashDisplay;
		GateTimer<> splashScreenPersistance;
		GameState* menuState;

		std::shared_ptr<AudioDelegate> baka, baka2;

		void initialise() override;
	};
}