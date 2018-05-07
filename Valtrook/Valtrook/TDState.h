#pragma once

#include "GameState.h"

#include "GUI.hpp"

namespace Val {
	class Game;
	class TDState : public GameState {
	public:
		TDState(Game * const game);

		void initialise();

		void update(const TimingType& deltaTime) override;
		void render(const TimingType& deltaTime, RenderingEngine * const engine) override;

		GUIWindow window;
	};
}