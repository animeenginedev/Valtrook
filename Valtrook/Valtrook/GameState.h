#pragma once

#include "TimingType.h"
#include <SDL2\SDL.h>
#include "RegisterToScript.h"
#include "ScriptWrapper.h"
#include "VariableStore.h"
#include "GameObject.h"
#include <vector>
#include <unordered_map>
#include <string>

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

		GameObject* createGameObject(std::string scriptName);
		GameObject* getFirstGameObjectOfType(std::string scriptName);
		std::vector<GameObject*> getGameObjectsOfScriptType(std::string scriptName);
		void destroyGameObject(GameObject* gameObj);

		//Can fail
		bool addNamedGameObject(std::string name, GameObject* gameObj);
		//Can't fail
		void setNamedGameObject(std::string name, GameObject* gameObj);
		GameObject* getNamedGameObject(std::string name);
		void removeNamedGameObject(std::string name);

		VariableStore<int>* getIntStore();
		VariableStore<float>* getFloatStore();
	protected:
		std::vector<GameObject> gameObjects;
		std::unordered_map<std::string, ScriptWrapper> loadedScripts;

		Game *const game;
		bool bReloadScript;
		void reloadScriptImpl();

		int reloadKey;

		std::function<void(float)> updateFunc;
		std::function<void(float, RenderingEngine*)> renderFunc;

		ScriptWrapper stateScript;

		VariableStore<int> intStore;
		VariableStore<float> floatStore;
		VariableStore<GameObject*> gameObjectStore;
	};
}