#pragma once

#include "GameState.h"

#include "Rectangle.h"
#include "TimingType.h"
#include "GateTimer.h"
#include "Colour.h"

#include <array>

namespace Val {
	class Game;

	//Handles it's own cleanup, just new it and it'll delete itself with the delegate function bound for game.
	class TransitionState : public GameState {
	public:
		static void destroyTransitionStateDelegate(TransitionState* del) {
			delete del;
		}

		void registerToScript(chaiscript::ChaiScript* script);

		TransitionState(Game * const game) : GameState(game) {};

		void initialise(GameState* nextState, TimingType transitionTime, Colour start, Colour end);

		void update(const TimingType& deltaTime) override;
		void render(const TimingType& deltaTime, RenderingEngine * const engine) override;
	protected:
		void initialise() override;

		GameState* nextState;
		GateTimer<> timeTillNextState;
		std::array<Colour, 2> colourTransition;
	};

}