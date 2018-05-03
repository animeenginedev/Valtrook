#pragma once

#include <string>
#include <unordered_map>
#include <chaiscript\chaiscript.hpp>

namespace Val {
	template<typename Var>
	class VariableStore {
	public:
		static void registerToScript(chaiscript::ChaiScript* scriptingEngine, std::string extension) {
			scriptingEngine->add(chaiscript::user_type<VariableStore<Var>>(), "VariableStore" + extension);

			scriptingEngine->add(chaiscript::fun(&VariableStore<Var>::setNamedVariable), "setNamed" + extension);
			scriptingEngine->add(chaiscript::fun(&VariableStore<Var>::getNamedVariable), "getNamed" + extension);
		}

		void setNamedVariable(std::string name, Var variable) {
			namedVariables.erase(name);
			namedVariables.insert(std::make_pair(name, variable));
		}

		Var getNamedVariable(std::string name) {
			if (namedVariables.find(name) != namedVariables.end())
				return namedVariables.at(name);
			return NULL;
		}

		void reset() {
			namedVariables.clear();
		}

	protected:
		std::unordered_map<std::string, Var> namedVariables;
	};
}