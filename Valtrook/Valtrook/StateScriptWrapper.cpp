#include "StateScriptWrapper.h"

#include "GameState.h"

namespace Val {
	StateScriptWrapper::StateScriptWrapper(std::string fileName, chaiscript::ChaiScript * scriptingCore) : ScriptWrapper(fileName, scriptingCore) {
	}
	StateScriptWrapper::~StateScriptWrapper() {
	}
	void StateScriptWrapper::reload() {
		ScriptWrapper::reload();

		initialiseFunc = getFunction<std::function<void()>>("initialise");
		onReloadScriptFunc = getFunction<std::function<void()>>("onReloadScript");
		updateFunc = getFunction<std::function<void(float)>>("update");
		renderFunc = getFunction<std::function<void(float)>>("render");
		onBecomeActiveFunc = getFunction<std::function<void()>>("onBecomeActive");
		onBecomeInactiveFunc = getFunction<std::function<void()>>("onBecomeInactive");
	}
}