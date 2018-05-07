#include "TDState.h"

#include "Game.h"

namespace Val {
	TDState::TDState(Game * const game) : GameState(game), window(game->getInputManager(), Camera::Cast<OrthographicCamera>(game->getCamera())) {
	}

	void TDState::initialise() {
	}

	void TDState::update(const TimingType & deltaTime) {
	}

	void TDState::render(const TimingType & deltaTime, RenderingEngine * const engine) {
	}
}