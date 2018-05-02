#include "BindingRegister.h"

#include "Engine.h"
#include "Logger.h"
#include "StopWatch.h"
#include "InputManager.h"
#include <string>
#include "Game.h"
#include "GameState.h"
#include "ResourceLocation.h"

template <typename t>
std::string to_string(t value) {
	return std::to_string(value);
}

void Val::BindingRegister::RegisterBindings(Engine* engine, chaiscript::ChaiScript& scriptingEngine) {
	//Resource
	ResourceLocation("", "", "").registerToScript(&scriptingEngine);

	//Logger
	Logger::Instance->logMessage(LogLevel::INFO, "Binding Chaiscript");
	Logger::Instance->outputLog();

	scriptingEngine.add(chaiscript::fun(&to_string<int>), "int_to_string");
	scriptingEngine.add(chaiscript::fun(&to_string<float>), "float_to_string");
	scriptingEngine.add(chaiscript::fun(&to_string<double>), "double_to_string");

	engine->registerToScript(&scriptingEngine);
	scriptingEngine.add_global(chaiscript::var(engine), "Engine");
	
	Logger::Instance->registerToScript(&scriptingEngine);
	scriptingEngine.add_global(chaiscript::var(Logger::Instance), "Logger");

	//Game
	engine->getGame()->registerToScript(&scriptingEngine);
	GameState(nullptr).registerToScript(&scriptingEngine);

	//Timing
	StopWatch().registerToScript(&scriptingEngine);

	//Input
	KeyState().registerToScript(&scriptingEngine);
	InputManager().registerToScript(&scriptingEngine);
	scriptingEngine.add_global(chaiscript::var(engine->getInputManager()), "InputManager");

	//Audio
	engine->getAudioManager()->registerToScript(&scriptingEngine);
	scriptingEngine.add_global(chaiscript::var(engine->getAudioManager()), "AudioManager");

	AudioAsset().registerToScript(&scriptingEngine);
	AudioDelegate(nullptr).registerToScript(&scriptingEngine);
}
