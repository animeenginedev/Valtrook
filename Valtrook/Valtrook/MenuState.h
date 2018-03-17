#pragma once

#include "GameState.h"

#include "Rectangle.h"

#include "GUI_VerticalBox.h"
#include "GUI_Label.h"
namespace Val {
	class Game;
	class MenuState : public GameState {
	public:
		MenuState(Game * const game) : GameState(game) {};

		void initialise();

		void update(const TimingType& deltaTime) override;
		void render(const TimingType& deltaTime, RenderingEngine * const engine) override;

		GUI_VerticalBox testBox;

		GUI_Label::Ptr testLabel;
	};
}