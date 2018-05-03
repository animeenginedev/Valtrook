#include "BindingRegister.h"

#include "Engine.h"
#include "RenderingEngine.h"
#include "Logger.h"
#include "StopWatch.h"
#include "InputManager.h"
#include <string>
#include "Game.h"
#include "GameState.h"
#include "TransitionState.h"
#include "ResourceLocation.h"
#include "GateTimer.h"
#include "FontAsset.h"
#include "TextResource.h"
#include "VariableStore.h"

#include "SimpleRectangle.h"
#include "Rectangle.h"
#include "SimpleTextRectangle.h"
#include "TextRectangle.h"
#include "SimpleMultilineTextRectangle.h"

template <typename t>
std::string to_string(t value) {
	return std::to_string(value);
}

void Val::BindingRegister::RegisterBindings(Engine* engine, chaiscript::ChaiScript& scriptingEngine) {
	//Resource
	ResourceLocation("", "", "").registerToScript(&scriptingEngine);

	VariableStore<int>::registerToScript(&scriptingEngine, "Int");
	VariableStore<float>::registerToScript(&scriptingEngine, "Float");

	//Timing
	GateTimer<float>().registerToScript(&scriptingEngine);


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
	TransitionState(nullptr).registerToScript(&scriptingEngine);

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

	//Rendering
	engine->getRenderingEngine()->registerToScript(&scriptingEngine);
	engine->getRenderingEngine()->getWindow().registerToScript(&scriptingEngine);
	Colour::registerToScript(&scriptingEngine);
	GLBlendMode::registerToScript(&scriptingEngine);
	FontAsset(0, nullptr).registerToScript(&scriptingEngine);
	TextResource(nullptr, "").registerToScript(&scriptingEngine);

	//Renderables

	ATexturedRect::registerToScript(&scriptingEngine);
	SimpleRectangle(TextureResource(nullptr, ResourceLocation("", "", ""))).registerToScript(&scriptingEngine);
	Rectangle(TextureResource(nullptr, ResourceLocation("", "", ""))).registerToScript(&scriptingEngine);
	SimpleTextRectangle(TextResource(nullptr, "")).registerToScript(&scriptingEngine);
	TextRectangle(TextResource(nullptr, "")).registerToScript(&scriptingEngine);
	SimpleMultilineTextRectangle(TextResource(nullptr, ""), { 0, 0 }, 0, 1.0f, 0.5f).registerToScript(&scriptingEngine);
}
