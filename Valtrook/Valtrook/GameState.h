#pragma once

#include "TimingType.h"
#include <SDL2\SDL.h>
#include "RegisterToScript.h"
#include "StateScriptWrapper.h"
#include "VariableStore.h"

namespace Val {
	class Game;
	class RenderingEngine;

	class GameState : public RegisterToScript {
	public:
		GameState(Game * const game, std::string scriptName = "");
		~GameState();

		void registerToScript(chaiscript::ChaiScript* script);

		virtual void initialise();
		virtual void reset();

		virtual void onBecomeActive();
		virtual void onBecomeInactive();

		virtual void rawEvent(const SDL_Event& e);
		virtual void update(const TimingType& deltaTime);
		virtual void render(const TimingType& deltaTime, RenderingEngine * const engine);

		void reloadScript();
		virtual void onReloadScript();

		void setReloadKey(int reloadKey);
		int getReloadKey();

		VariableStore<int>* getIntStore();
		VariableStore<float>* getFloatStore();
	protected:
		Game *const game;
		bool bReloadScript;
		void reloadScriptImpl();

		int reloadKey;

		std::function<void(float)> updateFunc, renderFunc;

		StateScriptWrapper stateScript;

		VariableStore<int> intStore;
		VariableStore<float> floatStore;
	};
}