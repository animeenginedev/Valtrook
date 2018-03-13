#include "SplashState.h"

#include "RuntimeConstants.h"
#include "TransitionState.h"
#include "MenuState.h"
#include "RenderingEngine.h"
#include "InputManager.h"
#include "Game.h"
#include "Angle.h"
#include "Conversion.h"

namespace Val {
	void SplashState::initialise() {
	}

	void SplashState::initialise(GameState * menuState) {
		splashScreenPersistance.setLength(static_cast<TimingType>(5.0));

		SplashDisplay.initialise(TextureAsset::getTexture(ResourceLocation("raven", ".png", RuntimeConstants::Instance->TexturePath)), 0, 0, 0.5f, PixelToWorld<int, float>(200), PixelToWorld<int, float>(200), 0.0f);

		this->menuState = menuState;
	}

	void SplashState::update(const TimingType & deltaTime) {
		if (splashScreenPersistance.update(deltaTime) || game->getInputManager()->isAnyJustUp()) {
			//Should never actually be true.
			if (game->hasNextState())
				return;
			auto TState = new TransitionState(game);
			TState->initialise(menuState, 0.50f, Colour(255, 255, 255, 255), Colour(0, 0, 0, 255));

			game->setState(TState);
		}
	}

	void SplashState::render(const TimingType & deltaTime, RenderingEngine * const engine) {
		SplashDisplay.setRotation(SplashDisplay.getRotation() + (Radians<TimingType>(TimingType(40.0f)) * deltaTime));

		SplashDisplay.sendRenderInformation(engine);
	}
}