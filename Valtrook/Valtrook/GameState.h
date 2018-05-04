#pragma once

#include "TimingType.h"
#include <SDL2\SDL.h>
#include "GameObject.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace Val {
	class Game;
	class RenderingEngine;

	class GameState {
	public:
		GameState(Game * const game, std::string scriptName = "");
		~GameState();

		virtual void initialise();
		virtual void reset();

		virtual void onBecomeActive();
		virtual void onBecomeInactive();

		virtual void rawEvent(const SDL_Event& e);
		virtual void update(const TimingType& deltaTime);
		virtual void render(const TimingType& deltaTime, RenderingEngine * const engine);

		GameObject* createGameObject(std::string scriptName);
		void destroyGameObject(GameObject* gameObj);
	protected:
		std::vector<GameObject> gameObjects;

		Game *const game;
	};
}