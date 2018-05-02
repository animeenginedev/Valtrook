#include "GameState.h"

#include "Game.h"
#include "InputManager.h"
#include "RenderingEngine.h"

Val::GameState::GameState(Game * const game, std::string scriptName) : game(game), stateScript(scriptName, game == nullptr ? nullptr : game->getScriptingEngine()), reloadKey(-1) {
} 

Val::GameState::~GameState() {
}

void Val::GameState::registerToScript(chaiscript::ChaiScript * script) {
	script->add(chaiscript::user_type<GameState>(), "GameState");

	script->add(chaiscript::fun(&GameState::initialise), "initialise");
	script->add(chaiscript::fun(&GameState::reset), "reset");
	script->add(chaiscript::fun(&GameState::reloadScript), "reloadScript");

	script->add(chaiscript::fun(&GameState::setReloadKey), "setReloadKey");
}

void Val::GameState::initialise() {
	if (stateScript.getFileName() != "") {
		stateScript.reload();
		stateScript.getScript()->add_global(chaiscript::var(this), "State");

		updateFunc = stateScript.getUpdateFunction();
		renderFunc = stateScript.getRenderFunction();
	}
	
	auto initFunc = stateScript.initialiseFunction();
	if (initFunc)
		stateScript.safetlyRunFunc(initFunc);
}

void Val::GameState::reset() {
}

void Val::GameState::onBecomeActive() {
	if (bReloadScript)
		reloadScriptImpl();

	auto activeFunc = stateScript.onBecomeActiveFunction();
	if (activeFunc)
		stateScript.safetlyRunFunc(activeFunc);
}

void Val::GameState::onBecomeInactive() {
	if (bReloadScript)
		reloadScriptImpl();

	auto inactiveFunc = stateScript.onBecomeInactiveFunction();
	if (inactiveFunc)
		stateScript.safetlyRunFunc(inactiveFunc);
}

void Val::GameState::rawEvent(const SDL_Event & e) {
}

void Val::GameState::update(const TimingType & deltaTime) {
	if (reloadKey != -1 && game->getInputManager()->isKeyJustDown(reloadKey)) 
		reloadScript();
	if (bReloadScript)
		reloadScriptImpl();

	if(updateFunc)
		stateScript.safetlyRunFunc(updateFunc, deltaTime);

}

void Val::GameState::render(const TimingType & deltaTime, Val::RenderingEngine * const engine) {
	if (bReloadScript)
		reloadScriptImpl();

	if (renderFunc)
		stateScript.safetlyRunFunc(renderFunc, deltaTime);
}

void Val::GameState::reloadScript() {
	bReloadScript = true;
}

void Val::GameState::reloadScriptImpl() {
	if (stateScript.getFileName() != "") {
		stateScript.reload();
		stateScript.getScript()->add_global(chaiscript::var(this), "State");

		updateFunc = stateScript.getUpdateFunction();
		renderFunc = stateScript.getRenderFunction();

		onReloadScript();
	}
	bReloadScript = false;
}

void Val::GameState::onReloadScript() {
	auto reloadScriptFunc = stateScript.onReloadScriptFunction();
	if (reloadScriptFunc)
		stateScript.safetlyRunFunc(reloadScriptFunc);
}

void Val::GameState::setReloadKey(int reloadKey) {
	this->reloadKey = reloadKey;
}

int Val::GameState::getReloadKey() {
	return reloadKey;
}
