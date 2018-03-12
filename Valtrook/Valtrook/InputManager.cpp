#include "InputManager.h"

#include <SDL2\SDL.h>

namespace Val {
	InputManager::InputManager() {
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
}