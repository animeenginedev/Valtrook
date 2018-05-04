#include "InputManager.h"

#include <SDL2\SDL.h>

namespace Val {
	InputManager::InputManager() : MouseState({ KeyState(false), KeyState(false), KeyState(false) }) {
		//probably enough to cover the vector being reexpanded at runtime.
		KeysToUpdate.reserve(100);
	}

	InputManager::~InputManager() {
	}
	void InputManager::update() {
		for (unsigned int counter = 0; counter != KeysToUpdate.size(); ++counter) {
			auto state = KeyStates.find(KeysToUpdate[counter]);
			state->second.set(state->second.bCurrent);
			//If this key doesn't need to be propergated again then we should remove it from the keys we want to update.
			if (!state->second.bCurrent && !state->second.bPrevious) {
				KeysToUpdate.erase(KeysToUpdate.begin() + counter);
				--counter;
			}
		}

		MouseState[0].set(MouseState[0].bCurrent);
		MouseState[1].set(MouseState[1].bCurrent);
		MouseState[2].set(MouseState[2].bCurrent);
		MouseDelta = 0;
	}
	void InputManager::updateKeyState(const unsigned short& key, const bool& newState) {
		if (KeyStates.find(key) == KeyStates.end()) {
			KeyStates.insert(std::make_pair(key, KeyState(newState)));
			if (newState == true)
				KeysToUpdate.push_back(key);
			return;
		}

		auto state = KeyStates.find(key);
		state->second.bCurrent = newState;
		if (newState == true && !state->second.bPrevious)
			KeysToUpdate.push_back(key);
	}
	void InputManager::updateMouseButton(const unsigned short& button, const bool& newState) {
		if (button == SDL_BUTTON_LEFT) {
			MouseState[0].set(newState);
		} else if (button == SDL_BUTTON_RIGHT) {
			MouseState[1].set(newState);
		} else if (button == SDL_BUTTON_MIDDLE) {
			MouseState[2].set(newState);
		}
	}
	void InputManager::updateMouseWheel(const int& newDelta) {
		MouseDelta = newDelta;
	}
	bool InputManager::isAnyJustUp() const {
		for (auto keyID : KeysToUpdate) {
			if (KeyStates.at(keyID).isKeyJustUp())
				return true;
		}
		for (unsigned int i = 0; i < 3; ++i) {
			if (MouseState[i].isKeyJustUp())
				return true;
		}
		return false;
	}
	bool InputManager::isAnyJustDown() const {
		for (auto keyID : KeysToUpdate) {
			if (KeyStates.at(keyID).isKeyJustDown())
				return true;
		}
		for (unsigned int i = 0; i < 3; ++i) {
			if (MouseState[i].isKeyJustDown())
				return true;
		}
		return false;
	}
	bool InputManager::isAnyPressed() const {
		for (auto keyID : KeysToUpdate) {
			if (KeyStates.at(keyID).isKeyPressed())
				return true;
		}
		for (unsigned int i = 0; i < 3; ++i) {
			if (MouseState[i].isKeyPressed())
				return true;
		}
		return false;
	}
	bool InputManager::isKeyJustUp(const unsigned short& key) const {
		if (KeyStates.find(key) == KeyStates.end()) {
			return false;
		}
		return KeyStates.at(key).isKeyJustUp();
	}

	bool InputManager::isKeyJustDown(const unsigned short& key) const {
		if (KeyStates.find(key) == KeyStates.end()) {
			return false;
		}
		return KeyStates.at(key).isKeyJustDown();
	}

	bool InputManager::isKeyPressed(const unsigned short& key) const {
		if (KeyStates.find(key) == KeyStates.end()) {
			return false;
		}
		return KeyStates.at(key).isKeyPressed();
	}
	const KeyState InputManager::getLeftMouseState() const {
		return MouseState[0];
	}

	const KeyState InputManager::getRightMouseState() const {
		return MouseState[1];
	}

	const KeyState InputManager::getMiddleMouseState() const {
		return MouseState[2];
	}

	int InputManager::getMouseWheelDelta() const {
		return MouseDelta;
	}

	std::array<int, 2> InputManager::getMouseLocation() const {
		int x, y;
		SDL_GetMouseState(&x, &y);
		return{ x, y };
	}
	void InputManager::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::fun(&InputManager::isAnyJustUp), "isAnyJustUp");
		script->add(chaiscript::fun(&InputManager::isAnyJustDown), "isAnyJustDown");
		script->add(chaiscript::fun(&InputManager::isAnyPressed), "isAnyPressed");

		script->add(chaiscript::fun(&InputManager::isKeyJustUp), "isKeyJustUp");
		script->add(chaiscript::fun(&InputManager::isKeyJustDown), "isKeyJustDown");
		script->add(chaiscript::fun(&InputManager::isKeyPressed), "isKeyPressed");

		script->add(chaiscript::fun(&InputManager::getLeftMouseState), "getLeftMouseState");
		script->add(chaiscript::fun(&InputManager::getRightMouseState), "getRightMouseState");
		script->add(chaiscript::fun(&InputManager::getMiddleMouseState), "getMiddleMouseState");
		script->add(chaiscript::fun(&InputManager::getMouseWheelDelta), "getMouseWheelDelta");
		script->add(chaiscript::fun(&InputManager::getMouseLocation), "getMouseLocation");		

		//KeyCode Registration (Fucking shoot me)
		script->add_global(chaiscript::var<int>(SDLK_0), "Key_0");
		script->add_global(chaiscript::var<int>(SDLK_1), "Key_1");
		script->add_global(chaiscript::var<int>(SDLK_2), "Key_2");
		script->add_global(chaiscript::var<int>(SDLK_3), "Key_3");
		script->add_global(chaiscript::var<int>(SDLK_4), "Key_4");
		script->add_global(chaiscript::var<int>(SDLK_5), "Key_5");
		script->add_global(chaiscript::var<int>(SDLK_6), "Key_6");
		script->add_global(chaiscript::var<int>(SDLK_7), "Key_7");
		script->add_global(chaiscript::var<int>(SDLK_8), "Key_8");
		script->add_global(chaiscript::var<int>(SDLK_9), "Key_9");

		script->add_global(chaiscript::var<int>(SDLK_a), "Key_a");
		script->add_global(chaiscript::var<int>(SDLK_b), "Key_b");
		script->add_global(chaiscript::var<int>(SDLK_c), "Key_c");
		script->add_global(chaiscript::var<int>(SDLK_d), "Key_d");
		script->add_global(chaiscript::var<int>(SDLK_e), "Key_e");
		script->add_global(chaiscript::var<int>(SDLK_f), "Key_f");
		script->add_global(chaiscript::var<int>(SDLK_g), "Key_g");
		script->add_global(chaiscript::var<int>(SDLK_h), "Key_h");
		script->add_global(chaiscript::var<int>(SDLK_i), "Key_i");
		script->add_global(chaiscript::var<int>(SDLK_j), "Key_j");
		script->add_global(chaiscript::var<int>(SDLK_k), "Key_k");
		script->add_global(chaiscript::var<int>(SDLK_l), "Key_l");
		script->add_global(chaiscript::var<int>(SDLK_m), "Key_m");
		script->add_global(chaiscript::var<int>(SDLK_n), "Key_n");
		script->add_global(chaiscript::var<int>(SDLK_o), "Key_o");
		script->add_global(chaiscript::var<int>(SDLK_p), "Key_p");
		script->add_global(chaiscript::var<int>(SDLK_q), "Key_q");
		script->add_global(chaiscript::var<int>(SDLK_r), "Key_r");
		script->add_global(chaiscript::var<int>(SDLK_s), "Key_s");
		script->add_global(chaiscript::var<int>(SDLK_t), "Key_t");
		script->add_global(chaiscript::var<int>(SDLK_u), "Key_u");
		script->add_global(chaiscript::var<int>(SDLK_v), "Key_v");
		script->add_global(chaiscript::var<int>(SDLK_w), "Key_w");
		script->add_global(chaiscript::var<int>(SDLK_x), "Key_x");
		script->add_global(chaiscript::var<int>(SDLK_y), "Key_y");
		script->add_global(chaiscript::var<int>(SDLK_z), "Key_z");

		script->add_global(chaiscript::var<int>(SDLK_EXCLAIM), "Key_Exclaim");
		script->add_global(chaiscript::var<int>(SDLK_AT), "Key_At");
		script->add_global(chaiscript::var<int>(SDLK_HASH), "Key_Hash");
		script->add_global(chaiscript::var<int>(SDLK_DOLLAR), "Key_Dollar");
		script->add_global(chaiscript::var<int>(SDLK_PERCENT), "Key_Percent");
		script->add_global(chaiscript::var<int>(SDLK_CARET), "Key_Caret");
		script->add_global(chaiscript::var<int>(SDLK_AMPERSAND), "Key_Ampersand");
		script->add_global(chaiscript::var<int>(SDLK_ASTERISK), "Key_Asterisk");

		script->add_global(chaiscript::var<int>(SDLK_MINUS), "Key_Minus");
		script->add_global(chaiscript::var<int>(SDLK_UNDERSCORE), "Key_Underscore");
		script->add_global(chaiscript::var<int>(SDLK_EQUALS), "Key_Equals");
		script->add_global(chaiscript::var<int>(SDLK_PLUS), "Key_Plus");
		script->add_global(chaiscript::var<int>(SDLK_BACKSPACE), "Key_Backspace");

		script->add_global(chaiscript::var<int>(SDLK_KP_LEFTPAREN), "Key_LParen");
		script->add_global(chaiscript::var<int>(SDLK_KP_RIGHTPAREN), "Key_RParen");
		script->add_global(chaiscript::var<int>(SDLK_KP_LEFTBRACE), "Key_LBrace");
		script->add_global(chaiscript::var<int>(SDLK_KP_RIGHTBRACE), "Key_RBrace");
		script->add_global(chaiscript::var<int>(SDLK_LEFTBRACKET), "Key_LBracket");
		script->add_global(chaiscript::var<int>(SDLK_RIGHTBRACKET), "Key_RBracket");
		
		script->add_global(chaiscript::var<int>(SDLK_BACKSLASH), "Key_Backslash");
		script->add_global(chaiscript::var<int>(SDLK_SLASH), "Key_Slash");
		script->add_global(chaiscript::var<int>(SDLK_RETURN), "Key_Return");
		script->add_global(chaiscript::var<int>(SDLK_RETURN2), "Key_Return2");

		script->add_global(chaiscript::var<int>(SDLK_COLON), "Key_Colon");
		script->add_global(chaiscript::var<int>(SDLK_SEMICOLON), "Key_Semicolon");
		script->add_global(chaiscript::var<int>(SDLK_QUOTE), "Key_Apostrophe");
		script->add_global(chaiscript::var<int>(SDLK_QUOTEDBL), "Key_Quote");
		script->add_global(chaiscript::var<int>(SDLK_COMMA), "Key_Comma");
		script->add_global(chaiscript::var<int>(SDLK_PERIOD), "Key_Period");
		script->add_global(chaiscript::var<int>(SDLK_QUESTION), "Key_Question");

		script->add_global(chaiscript::var<int>(SDLK_LEFT), "Key_Left");
		script->add_global(chaiscript::var<int>(SDLK_RIGHT), "Key_Right");
		script->add_global(chaiscript::var<int>(SDLK_UP), "Key_Up");
		script->add_global(chaiscript::var<int>(SDLK_DOWN), "Key_Down");

		script->add_global(chaiscript::var<int>(SDLK_SPACE), "Key_Space");
		script->add_global(chaiscript::var<int>(SDLK_LSHIFT), "Key_LShift");
		script->add_global(chaiscript::var<int>(SDLK_LCTRL), "Key_LControl");
		script->add_global(chaiscript::var<int>(SDLK_LALT), "Key_LAlt");
		script->add_global(chaiscript::var<int>(SDLK_RSHIFT), "Key_RShift");
		script->add_global(chaiscript::var<int>(SDLK_RCTRL), "Key_RControl");
		script->add_global(chaiscript::var<int>(SDLK_RALT), "Key_RAlt");
		script->add_global(chaiscript::var<int>(SDLK_TAB), "Key_Tab");
		script->add_global(chaiscript::var<int>(SDLK_CAPSLOCK), "Key_Capslock");

		script->add_global(chaiscript::var<int>(SDLK_ESCAPE), "Key_Escape");
		script->add_global(chaiscript::var<int>(SDLK_F1), "Key_F1");
		script->add_global(chaiscript::var<int>(SDLK_F2), "Key_F2");
		script->add_global(chaiscript::var<int>(SDLK_F3), "Key_F3");
		script->add_global(chaiscript::var<int>(SDLK_F4), "Key_F4");
		script->add_global(chaiscript::var<int>(SDLK_F5), "Key_F5");
		script->add_global(chaiscript::var<int>(SDLK_F6), "Key_F6");
		script->add_global(chaiscript::var<int>(SDLK_F7), "Key_F7");
		script->add_global(chaiscript::var<int>(SDLK_F8), "Key_F8");
		script->add_global(chaiscript::var<int>(SDLK_F9), "Key_F9");
		script->add_global(chaiscript::var<int>(SDLK_F10), "Key_F10");
		script->add_global(chaiscript::var<int>(SDLK_F11), "Key_F11");
		script->add_global(chaiscript::var<int>(SDLK_F12), "Key_F12");

		script->add_global(chaiscript::var<int>(SDLK_INSERT), "Key_Insert");
		script->add_global(chaiscript::var<int>(SDLK_HOME), "Key_Home");
		script->add_global(chaiscript::var<int>(SDLK_PAGEUP), "Key_PageUp");
		script->add_global(chaiscript::var<int>(SDLK_PAGEDOWN), "Key_PageDown");
		script->add_global(chaiscript::var<int>(SDLK_END), "Key_End");
		script->add_global(chaiscript::var<int>(SDLK_DELETE), "Key_Delete");

		script->add_global(chaiscript::var<int>(SDLK_KP_0), "Key_KP0");
		script->add_global(chaiscript::var<int>(SDLK_KP_1), "Key_KP1");
		script->add_global(chaiscript::var<int>(SDLK_KP_2), "Key_KP2");
		script->add_global(chaiscript::var<int>(SDLK_KP_3), "Key_KP3");
		script->add_global(chaiscript::var<int>(SDLK_KP_4), "Key_KP4");
		script->add_global(chaiscript::var<int>(SDLK_KP_5), "Key_KP5");
		script->add_global(chaiscript::var<int>(SDLK_KP_6), "Key_KP6");
		script->add_global(chaiscript::var<int>(SDLK_KP_7), "Key_KP7");
		script->add_global(chaiscript::var<int>(SDLK_KP_8), "Key_KP8");
		script->add_global(chaiscript::var<int>(SDLK_KP_9), "Key_KP9");

		script->add_global(chaiscript::var<int>(SDLK_KP_PLUS), "Key_KP_Plus");
		script->add_global(chaiscript::var<int>(SDLK_KP_MINUS), "Key_KP_Minus");
		script->add_global(chaiscript::var<int>(SDLK_KP_MULTIPLY), "Key_KP_Multiply");
		script->add_global(chaiscript::var<int>(SDLK_KP_DIVIDE), "Key_KP_Divide");
		script->add_global(chaiscript::var<int>(SDLK_KP_ENTER), "Key_KP_Enter");
	}
}