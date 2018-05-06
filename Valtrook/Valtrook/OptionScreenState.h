#pragma once

#include "GameState.h"

#include "ParticleSystem.h"
#include "GUI.hpp"

namespace Val {
	class Game;
	class OptionScreenState : public GameState {
	public:
		OptionScreenState(Game * const game);

		void initialise();

		void update(const TimingType& deltaTime) override;
		void render(const TimingType& deltaTime, RenderingEngine * const engine) override;

		GUIWindow window;
		ParticleSystem backgroundSystem;
	};
}