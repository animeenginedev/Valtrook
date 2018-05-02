#pragma once

#include "OrthographicCamera.h"
#include "TimingType.h"

#include <functional>
#include <SDL2\SDL.h>
#include <chaiscript\chaiscript.hpp>
#include "RegisterToScript.h"
#include <unordered_map>
#include "ScriptWrapper.h"

namespace Val {
	class Camera;
	class GameState;
	class RenderingEngine;
	class InputManager;
	class AudioManager;

	class SplashState;
	class MenuState;

	class Game : public RegisterToScript {
	public:
		Game(InputManager const* const manager, AudioManager* audioManager, chaiscript::ChaiScript* scriptingEngine);
		~Game();

		void initialise();

		void rawEvent(const SDL_Event& e);
		void update(const TimingType& deltaTime);
		void render(const TimingType& deltaTime, RenderingEngine * const engine);

		InputManager const* const getInputManager() const;
		AudioManager * getAudioManager() const;
		chaiscript::ChaiScript* getScriptingEngine();

		GameState* getState() const;
		//happens at the end of the next update(), callback is set to null when it's completed.
		void setState(GameState* state, std::function<void()> Callback = nullptr);
		bool hasNextState() const;

		void setCamera(Camera* Camera);
		Camera* getCamera();
		void resetToDefaultCamera();

		GameState* getNamedState(const std::string name) const;
		void addNamedState(const std::string scriptName, const std::string name);
		void setNamedState(const std::string name);

		SplashState* splashState;
		MenuState* menuState;

		void registerToScript(chaiscript::ChaiScript* script);
	protected:
		std::unordered_map<std::string, GameState*> namedStates;

		GameState* currentState;
		GameState* nextState;
		std::function<void()> callbackOnStateChange;
		Camera* currentCamera, *defaultCamera;
		InputManager const* const inputManager;
		AudioManager * audioManager;
		chaiscript::ChaiScript* scriptingEngine;

		ScriptWrapper entryPointScript;
	};

}