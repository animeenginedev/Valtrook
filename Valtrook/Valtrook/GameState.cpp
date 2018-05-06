#include "GameState.h"

#include "Game.h"
#include "InputManager.h"
#include "RenderingEngine.h"
#include "RuntimeConstants.h"


namespace Val {
	GameState::GameState(Game * const game) : game(game), gameObjects() {
	}

	GameState::~GameState() {
	}


	void GameState::initialise() {
	}

	void GameState::reset() {
		gameObjects.clear();
	}

	void GameState::onBecomeActive() {
	}

	void GameState::onBecomeInactive() {
	}

	void GameState::rawEvent(const SDL_Event & e) {
	}

	void GameState::update(const TimingType & deltaTime) {
		for (auto& obj : gameObjects)
			if (obj.isValid())
				obj.update(deltaTime);
		
	}

	void GameState::render(const TimingType & deltaTime, RenderingEngine * const engine) {
		for (auto& obj : gameObjects)
			if(obj.isValid()) 
				obj.render(deltaTime, engine);
	}

	GameObject * GameState::createGameObject(std::string scriptName) {		
		/*for (int i = 0; i < gameObjects.size(); ++i) {
			if (gameObjects[i].canBeReplaced()) {
				gameObjects[i] = GameObject(ScriptWrapper(scriptName, game->getScriptingEngine()), this);

				gameObjects[i].create();
				gameObjects[i].initialise();

				return &gameObjects[i];
			}
		}*/

		//gameObjects.push_back(GameObject(ScriptWrapper(scriptName, game->getScriptingEngine()), this));

		gameObjects.back().create();
		gameObjects.back().initialise();

		return &gameObjects.back();
	}

	void GameState::destroyGameObject(GameObject * gameObj) {
		if (gameObj == nullptr)
			return;

		if (gameObj->getGameObjectName() != "") {
			//gameObjectStore.removeNamedVariable(gameObj->getGameObjectName());
		}

		(&gameObjects[0] + ((gameObj - &gameObjects[0]) / sizeof(GameObject)))->destroy();
	}
}