#include "SplashState.h"

#include "RuntimeConstants.h"
#include "TransitionState.h"
#include "MenuState.h"
#include "RenderingEngine.h"
#include "InputManager.h"
#include "Game.h"
#include "Angle.h"
#include "Conversion.h"

#include "FontAsset.h"
#include "TextRenderer.h"

namespace Val {
	void SplashState::initialise() {
	}

	SplashState::SplashState(Game * const game) : GameState(game), SplashDisplay(TextResource(FontAsset::getFont(ResourceLocation("ralewaymed", ".ttf", RuntimeConstants::Instance->FontPath), 48), "SPLASH TEXT")) {
	}

	void SplashState::initialise(GameState * menuState) {
		splashScreenPersistance.setLength(static_cast<TimingType>(5.0));

		SplashDisplay = TextRectangle(TextResource(FontAsset::getFont(ResourceLocation("ralewaymed", ".ttf", RuntimeConstants::Instance->FontPath), 48), "SPLASH TEXT"), 0, 0, 0.5f, PixelToWorld<int, float>(200), PixelToWorld<int, float>(24), 0.0f);

		this->menuState = menuState;

		baka = game->getAudioManager()->getAudioPlayer(ResourceLocation("bakabakabaka", ".ogg", RuntimeConstants::Instance->SoundPath));
		baka2 = game->getAudioManager()->getAudioPlayer(ResourceLocation("splashNoise", ".mp3", RuntimeConstants::Instance->SoundPath));
	}

	void SplashState::onBecomeActive() {
		//fade in has a serious flaw, you cannot control the upper bound the audio goes too. so it goes from 0->128 regardless of channel settings.
		baka->playFadeIn(60000, nullptr, -1);
		baka2->play();
		baka2->setVolume(0.15f);
	}

	void SplashState::onBecomeInactive() {
		baka->halt();
		baka2->halt();
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