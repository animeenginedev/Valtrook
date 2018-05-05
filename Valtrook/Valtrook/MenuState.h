#pragma once

#include "GameState.h"

#include "SimpleRectangle.h"

#include "ParticleSystem.h"
#include "GUI.hpp"

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

		ParticleSystem TestSystem;
		GUI_VerticalScroll::Ptr testScroll;
		GUIFrame::Ptr testFrame;
	};
}