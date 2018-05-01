#pragma once

#include <array>
#include <vector>
#include <unordered_map>

#include "RegisterToScript.h"

namespace Val {
	struct KeyState : RegisterToScript {
		KeyState() : bPrevious(false), bCurrent(true) {};
		KeyState(bool current) : bPrevious(false), bCurrent(current) {};

		bool bPrevious;
		bool bCurrent;

		void set(const bool& current) {
			this->bPrevious = this->bCurrent;
			this->bCurrent = current;
		}

		bool isKeyJustUp() const {
			return bPrevious && !bCurrent;
		}
		bool isKeyJustDown() const {
			return !bPrevious && bCurrent;
		}
		bool isKeyPressed() const {
			return bCurrent;
		}

		void registerToScript(chaiscript::ChaiScript* script) override {
			script->add(chaiscript::user_type<KeyState>(), "KeyState");

			script->add(chaiscript::fun(&KeyState::isKeyJustUp), "isKeyJustUp");
			script->add(chaiscript::fun(&KeyState::isKeyJustDown), "isKeyJustDown");
			script->add(chaiscript::fun(&KeyState::isKeyPressed), "isKeyPressed");
		}
	};

	class InputManager : RegisterToScript {
	public:
		InputManager();
		~InputManager();

		void update();
		void updateKeyState(const unsigned short& key, const bool& newState);
		void updateMouseButton(const unsigned short& button, const bool& newState);
		void updateMouseWheel(const int& newDelta);

		//Only truely valid in Update()
		bool isAnyJustUp() const;
		bool isAnyJustDown() const;
		bool isAnyPressed() const;
		//Only truely valid in Update()
		bool isKeyJustUp(const unsigned short& key) const;
		bool isKeyJustDown(const unsigned short& key) const;
		bool isKeyPressed(const unsigned short& key) const;
		//isKeyJustUp is only truely valid in Update()
		const KeyState getLeftMouseState() const;
		//isKeyJustUp is only truely valid in Update()
		const KeyState getRightMouseState() const;
		//isKeyJustUp is only truely valid in Update()
		const KeyState getMiddleMouseState() const;
		int getMouseWheelDelta() const;
		std::array<int, 2> getMouseLocation() const;

		void registerToScript(chaiscript::ChaiScript* script) override;
	protected:
		std::vector<unsigned short> KeysToUpdate;
		std::unordered_map<unsigned short, KeyState> KeyStates;
		int MouseDelta;
		std::array<KeyState, 3> MouseState;
	};

}