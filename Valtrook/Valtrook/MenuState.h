#pragma once

#include "GameState.h"

#include "SimpleRectangle.h"

#include "GUI.hpp"
#include <chaiscript\chaiscript.hpp>

namespace Val {
	class Game;
	class MenuState : public GameState {
	public:
		MenuState(Game * const game);

		void initialise();

		void update(const TimingType& deltaTime) override;
		void render(const TimingType& deltaTime, RenderingEngine * const engine) override;

		GUIWindow window;

		SimpleRectangle* cullingTest, *overlay;

		GUI_VerticalScroll::Ptr testScroll;
		GUIFrame::Ptr testFrame;


		std::function<void()> updateFunc;
		chaiscript::ChaiScript updateScript;
	};
}