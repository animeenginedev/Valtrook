#pragma once

#include <chaiscript\chaiscript.hpp>

namespace Val {
	class Engine;

	class BindingRegister {
	public:
		static void RegisterBindings(Engine* engine, chaiscript::ChaiScript& scriptingEngine);
	};
}