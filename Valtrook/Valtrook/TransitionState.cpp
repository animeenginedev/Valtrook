#include "TransitionState.h"

#include "Game.h"
#include "RuntimeConstants.h"
#include <GL\GL.h>

namespace Val {
	void TransitionState::initialise(GameState * nextState, TimingType transitionTime, Colour start, Colour end) {
		this->nextState = nextState;
		timeTillNextState.setLength(transitionTime);
		this->colourTransition = { start, end };
	}
	void TransitionState::update(const TimingType & deltaTime) {
	}
	void TransitionState::render(const TimingType & deltaTime, RenderingEngine * const engine) {
		Colour clearColour;
		if (timeTillNextState.update(deltaTime)) {
			clearColour = RuntimeConstants::Instance->GLClearColour.get();
			glClearColor(static_cast<float>(clearColour.r) / 255.0f, static_cast<float>(clearColour.g) / 255.0f, static_cast<float>(clearColour.b) / 255.0f, static_cast<float>(clearColour.a) / 255.0f);

			game->setState(nextState, std::bind(&destroyTransitionStateDelegate, this));
			return;
		}
		clearColour = Colour::lerp(colourTransition[0], colourTransition[1], timeTillNextState.getProgress());
		glClearColor(static_cast<float>(clearColour.r) / 255.0f, static_cast<float>(clearColour.g) / 255.0f, static_cast<float>(clearColour.b) / 255.0f, static_cast<float>(clearColour.a) / 255.0f);
	}
	void TransitionState::initialise() {}
}