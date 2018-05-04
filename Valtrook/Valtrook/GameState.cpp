#include "GameState.h"

#include "Game.h"
#include "InputManager.h"
#include "RenderingEngine.h"
#include "RuntimeConstants.h"


namespace Val {
	GameState::GameState(Game * const game, std::string scriptName) : game(game), stateScript(scriptName, game == nullptr ? nullptr : game->getScriptingEngine()), reloadKey(-1), gameObjects() {
	}

	GameState::~GameState() {
	}

	void GameState::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<GameState>(), "GameState");

		script->add(chaiscript::fun(&GameState::initialise), "initialise");
		script->add(chaiscript::fun(&GameState::reset), "reset");
		script->add(chaiscript::fun(&GameState::reloadScript), "reloadScript");

		script->add(chaiscript::fun(&GameState::createGameObject), "createGameObject");
		script->add(chaiscript::fun(&GameState::getFirstGameObjectOfType), "getFirstGameObjectOfType");
		script->add(chaiscript::fun(&GameState::getGameObjectsOfScriptType), "getGameObjectsOfType");
		script->add(chaiscript::fun(&GameState::destroyGameObject), "destroyGameObject");

		script->add(chaiscript::fun(&GameState::addNamedGameObject), "addNamedGameObject");
		script->add(chaiscript::fun(&GameState::setNamedGameObject), "setNamedGameObject");
		script->add(chaiscript::fun(&GameState::getNamedGameObject), "getNamedGameObject");
		script->add(chaiscript::fun(&GameState::removeNamedGameObject), "removeNamedGameObject");

		script->add(chaiscript::fun(&GameState::getIntStore), "getIntStore");
		script->add(chaiscript::fun(&GameState::getFloatStore), "getFloatStore");

		script->add(chaiscript::fun(&GameState::setReloadKey), "setReloadKey");
		script->add(chaiscript::fun(&GameState::getReloadKey), "getReloadKey");
	}

	void GameState::initialise() {
		if (stateScript.getFileName() != "") {
			stateScript.reload();
			stateScript.getScript()->add_global(chaiscript::var(this), "State");

			updateFunc = stateScript.getFunction<void(float)>("update");
			renderFunc = stateScript.getFunction<void(float, RenderingEngine*)>("render");
		}

		auto initFunc = stateScript.getFunction<void()>("initialise");
		if (initFunc)
			stateScript.safetlyRunFunc(initFunc);
	}

	void GameState::reset() {
		reloadKey = -1;
		intStore.reset();
		floatStore.reset();
		gameObjectStore.reset();
		gameObjects.clear();
		loadedScripts.clear();
	}

	void GameState::onBecomeActive() {
		if (bReloadScript)
			reloadScriptImpl();

		auto activeFunc = stateScript.getFunction<void()>("onBecomeActive");
		if (activeFunc)
			stateScript.safetlyRunFunc(activeFunc);
	}

	void GameState::onBecomeInactive() {
		if (bReloadScript)
			reloadScriptImpl();

		auto inactiveFunc = stateScript.getFunction<void()>("onBecomeInactive");
		if (inactiveFunc)
			stateScript.safetlyRunFunc(inactiveFunc);
	}

	void GameState::rawEvent(const SDL_Event & e) {
	}

	void GameState::update(const TimingType & deltaTime) {
		if (reloadKey != -1 && game->getInputManager()->isKeyJustDown(reloadKey))
			reloadScript();
		if (bReloadScript)
			reloadScriptImpl();

		for (auto& obj : gameObjects)
			if (obj.isValid())
				obj.update(deltaTime);

		if (updateFunc)
			stateScript.safetlyRunFunc(updateFunc, deltaTime);

	}

	void GameState::render(const TimingType & deltaTime, RenderingEngine * const engine) {
		if (bReloadScript)
			reloadScriptImpl();

		for (auto& obj : gameObjects)
			if(obj.isValid()) 
				obj.render(deltaTime, engine);

		if (renderFunc)
			stateScript.safetlyRunFunc(renderFunc, deltaTime, engine);
	}

	void GameState::reloadScript() {
		bReloadScript = true;
	}

	void GameState::reloadScriptImpl() {
		if (stateScript.getFileName() != "") {
			stateScript.reload();
			stateScript.getScript()->add_global(chaiscript::var(this), "State");

			updateFunc = stateScript.getFunction<void(float)>("update");
			renderFunc = stateScript.getFunction<void(float, RenderingEngine*)>("render");

			onReloadScript();
		}
		bReloadScript = false;
	}

	void GameState::onReloadScript() {
		auto reloadScriptFunc = stateScript.getFunction<void()>("onReloadScript");
		if (reloadScriptFunc)
			stateScript.safetlyRunFunc(reloadScriptFunc);
	}

	void GameState::setReloadKey(int reloadKey) {
		this->reloadKey = reloadKey;
	}

	int GameState::getReloadKey() {
		return reloadKey;
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

		gameObjects.push_back(GameObject(ScriptWrapper(scriptName, game->getScriptingEngine()), this));

		gameObjects.back().create();
		gameObjects.back().initialise();

		return &gameObjects.back();
	}

	GameObject * GameState::getFirstGameObjectOfType(std::string scriptName) {
		scriptName = RuntimeConstants::Instance->ScriptingPath + scriptName;
		for (auto& obj : gameObjects) {
			if (obj.getScriptName() == scriptName)
				return &obj;
		}
		return nullptr;
	}

	std::vector<GameObject*> GameState::getGameObjectsOfScriptType(std::string scriptName) {
		scriptName = RuntimeConstants::Instance->ScriptingPath + scriptName;
		auto objs =  std::vector<GameObject*>();
		for (auto& obj : gameObjects) {
			if (obj.getScriptName() == scriptName) {
				objs.push_back(&obj);
			}
		}
		return objs;
	}

	void GameState::destroyGameObject(GameObject * gameObj) {
		if (gameObj == nullptr)
			return;

		if (gameObj->getGameObjectName() != "") {
			gameObjectStore.removeNamedVariable(gameObj->getGameObjectName());
		}

		(&gameObjects[0] + ((gameObj - &gameObjects[0]) / sizeof(GameObject)))->destroy();
	}

	bool GameState::addNamedGameObject(std::string name, GameObject * gameObj) {
		if (gameObjectStore.getNamedVariable(name) == nullptr) {
			gameObj->setGameObjectName(name);
			gameObjectStore.setNamedVariable(name, gameObj);
			return true;
		}
		return false;
	}

	void GameState::setNamedGameObject(std::string name, GameObject * gameObj) {
		if (gameObj == nullptr)
			return;
		removeNamedGameObject(name);
		gameObj->setGameObjectName(name);
		gameObjectStore.setNamedVariable(name, gameObj);
	}

	GameObject * GameState::getNamedGameObject(std::string name) {
		return gameObjectStore.getNamedVariable(name);
	}

	void GameState::removeNamedGameObject(std::string name) {
		if (gameObjectStore.getNamedVariable(name) != nullptr) {
			auto obj = gameObjectStore.getNamedVariable(name);
			obj->setGameObjectName("");
			gameObjectStore.removeNamedVariable(name);
		}
	}

	VariableStore<int>* GameState::getIntStore() {
		return &intStore;
	}

	VariableStore<float>* GameState::getFloatStore() {
		return &floatStore;
	}
}