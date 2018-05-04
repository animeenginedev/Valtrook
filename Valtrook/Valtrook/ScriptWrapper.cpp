#include "ScriptWrapper.h"

#include "RuntimeConstants.h"
#include "Logger.h"

namespace Val {
	ScriptWrapper::ScriptWrapper(std::string fileName, chaiscript::ChaiScript * scriptingCore) : ScriptWrapper(fileName, scriptingCore, true) {
	}
	ScriptWrapper::ScriptWrapper(std::string fileName, chaiscript::ChaiScript * scriptingCore, bool includeFilePathInnatly) : fileName(includeFilePathInnatly ? RuntimeConstants::Instance->ScriptingPath + fileName : fileName),
		scriptingData(new chaiscript::ChaiScript()), scriptingCore(scriptingCore), bValid(false) {
	}
	ScriptWrapper::~ScriptWrapper() {
	}
	void ScriptWrapper::reload() {
		try {
			scriptingData->set_state(scriptingCore->get_state());
			scriptingData->set_locals(scriptingCore->get_locals());
			scriptingData->eval_file(fileName);
			bValid = true;
		} catch(const std::exception &e) {
			Logger::Instance->logMessage(LogLevel::WARNING, e.what());
			bValid = false;
		}
	}
	bool ScriptWrapper::isValid() const {
		return bValid;
	}
	std::string ScriptWrapper::getFileName() const {
		return fileName;
	}
	chaiscript::ChaiScript * ScriptWrapper::getScript() {
		return scriptingData.get();
	}
}