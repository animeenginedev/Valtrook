#pragma once

#include "ScriptWrapper.h"

namespace Val {
	class GameState;

	class StateScriptWrapper : public ScriptWrapper {
	public:
		StateScriptWrapper(std::string fileName, chaiscript::ChaiScript* scriptingCore);
		~StateScriptWrapper();

		inline std::function<void()> initialiseFunction() { return initialiseFunc; }
		inline std::function<void()> onReloadScriptFunction() { return onReloadScriptFunc; }
		inline std::function<void(float)> getUpdateFunction() { return updateFunc; }
		inline std::function<void(float)> getRenderFunction() { return renderFunc; }
		inline std::function<void()> onBecomeActiveFunction() { return onBecomeActiveFunc; }
		inline std::function<void()> onBecomeInactiveFunction() { return onBecomeInactiveFunc; }

		void reload() override;
	protected:
		std::function<void()> initialiseFunc, onReloadScriptFunc;
		std::function<void()> onBecomeActiveFunc, onBecomeInactiveFunc;
		std::function<void(float)> updateFunc, renderFunc;
	};

}