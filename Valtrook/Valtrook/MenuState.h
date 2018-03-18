#pragma once

#include "GameState.h"

#include "Rectangle.h"

#include "GUI.hpp"

namespace Val {
	class Game;
	class MenuState : public GameState {
	public:
		MenuState(Game * const game) : GameState(game) {};

		void initialise();

		void update(const TimingType& deltaTime) override;
		void render(const TimingType& deltaTime, RenderingEngine * const engine) override;

		GUIWindow window;

		GUIFrame::Ptr testFrame;
	};
}