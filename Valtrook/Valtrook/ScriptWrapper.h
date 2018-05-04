#pragma once

#include <chaiscript\chaiscript.hpp>
#include <vector>
#include <exception>
#include <memory>

namespace Val {
	class ScriptWrapper {
	public:
		ScriptWrapper(const ScriptWrapper& rhs) : fileName(rhs.fileName), bValid(rhs.bValid), scriptingCore(rhs.scriptingCore), scriptingData(rhs.scriptingData) {};
		ScriptWrapper(std::string fileName, chaiscript::ChaiScript* scriptingCore);
		ScriptWrapper(std::string fileName, chaiscript::ChaiScript* scriptingCore, bool includeFilePathInnatly);
		~ScriptWrapper();

		template<typename T>
		std::function<T> getFunction(std::string funcName) {
			try {
				return scriptingData->eval<std::function<T>>(funcName);
			} catch(const chaiscript::exception::bad_boxed_cast& b) {
				printf(std::string("[INFO] :: Bad Boxed Cast @" + fileName + "::" + funcName + "\n" + b.what() + "\n").c_str());
			} catch (const chaiscript::exception::eval_error& e) {
				//I don't think this actually ever fires, errors on eval<T> only turn up when trying to run the function.
				//Can't include logger because circular inclusion memes
				printf(std::string("[INFO] :: Script Function Not Found or Invalid @" + fileName + "::" + funcName + "\n " + e.pretty_print() + "\n").c_str());
			} catch (const std::exception& except) {
				printf(std::string("[INFO] :: Something Failed @" + fileName + "::" + funcName + "\n" + except.what() + "\n").c_str());
			}
			return std::function<T>();
		}
		
		template<typename T, typename... Args>
		void safetlyRunFunc(T& func, Args... args) {
			try {
				func(args...);
				return;
			} catch (const chaiscript::exception::eval_error& e) {
				//Can't include logger because circular inclusion memes
				printf(std::string("[INFO] :: Script Function Not Found or Invalid @" + fileName + "\n" + e.pretty_print() + "\n").c_str());
			} catch (const std::exception& except) {
				printf(std::string("[INFO] :: Something Failed @" + fileName + "::\n" + except.what() + "\n").c_str());
			}
			func = nullptr;
		}
		
		virtual void reload();

		bool isValid() const;

		std::string getFileName() const;
		chaiscript::ChaiScript* getScript();
	protected:
		bool bValid;
		std::string fileName;
		std::shared_ptr<chaiscript::ChaiScript> scriptingData;

		chaiscript::ChaiScript* scriptingCore;
	};
}