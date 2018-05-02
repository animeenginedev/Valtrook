#pragma once

#include <chaiscript\chaiscript.hpp>
#include <vector>
#include <exception>

namespace Val {
	class ScriptWrapper {
	public:
		ScriptWrapper(std::string fileName, chaiscript::ChaiScript* scriptingCore);
		~ScriptWrapper();

		template<typename T>
		T getFunction(std::string funcName) {
			try {
				return scriptingData.eval<T>(funcName);
			} catch(const chaiscript::exception::bad_boxed_cast& b) {
				printf(std::string("[INFO] :: Bad Boxed Cast: " + funcName + "\n" + b.what() + "\n").c_str());
				return T();
			} catch (const chaiscript::exception::eval_error& e) {
				//I don't think this actually ever fires, errors on eval<T> only turn up when trying to run the function.
				//Can't include logger because circular inclusion memes
				printf(std::string("[INFO] :: Script Function Not Found or Invalid: " + funcName + "\n " + e.pretty_print() + "\n").c_str());
				return T();
			} catch (const std::exception& except) {
				printf(std::string("[INFO] :: Something Failed: " + funcName + "\n" + except.what() + "\n").c_str());
				return T();
			}
		}

		template<typename T, typename... Args>
		void safetlyRunFunc(T& func, Args... args) {
			try {
				func(args...);
				return;
			} catch (const chaiscript::exception::eval_error& e) {
				//Can't include logger because circular inclusion memes
				printf(std::string("[INFO] :: Script Function Not Found or Invalid\n" + e.pretty_print() + "\n").c_str());
			}
			func = nullptr;
		}

		virtual void reload();

		bool isValid();

		std::string getFileName() const;
		chaiscript::ChaiScript* getScript();
	protected:
		bool bValid;
		std::string fileName;
		chaiscript::ChaiScript scriptingData;

		chaiscript::ChaiScript* scriptingCore;
	};
}