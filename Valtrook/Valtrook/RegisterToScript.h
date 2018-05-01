#pragma once

#include <chaiscript\chaiscript.hpp>

namespace Val {
	class RegisterToScript {
	public:
		virtual void registerToScript(chaiscript::ChaiScript* script) = 0;
	};
}