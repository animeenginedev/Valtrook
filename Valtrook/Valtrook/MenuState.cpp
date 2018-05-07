#include "MenuState.h"

#include "RuntimeConstants.h"
#include "Conversion.h"

#include "Game.h"
#include "InputManager.h"
#include "Engine.h"
#include "Logger.h"
#include "OrthographicCamera.h"
#include "AudioManager.h"
#include "Owner.h"
#include "RenderingEngine.h"
#include "OptionScreenState.h"
#include "TDState.h"
#include "Window.h"
#include "Particle.h"
#include "ParticleFunctions.h"

namespace Val {
	MenuState::MenuState(Game * const game) : GameState(game), window(game->getInputManager(), Camera::Cast<OrthographicCamera>(game->getCamera())) {
	}

	GUI_Button::Ptr createButton(std::string name) {
		auto gamesText = SimpleMultilineTextRectangle(TextResource(name, "ralewaymed", 128),
		{ 1, 1 }, 0.5f, 1.0f, 0.25f, Colour(0, 0, 0, 255));
		auto gamesLabel = GUI_Label::Create(gamesText);
		gamesLabel->setRecievesInputs(false);
		auto gamePadding = GUI_Padding::Create(0.05f, 0.05f);
		gamePadding->addChild(gamesLabel);
		gamePadding->setRecievesInputs(false);
		auto gameButton = GUI_Button::Create(0.15f, Colour(100, 100, 100, 255), Colour(80, 80, 80, 255), Colour(50, 50, 50, 255),
											 Colour(230, 230, 230, 255), Colour(200, 200, 200, 255), Colour(150, 150, 150, 255), FrameStyle::getDefaultCurved());
		gameButton->addChild(gamePadding);

		return gameButton;
	}

	void MenuState::initialise() {
		Engine::Instance->setTargetUpdateRate(60);
		Engine::Instance->setTargetFrameRate(120);
		RuntimeConstants::Instance->WindowName.set("Kadzu Game");

		testFrame = GUIFrame::Create(1);
		window.addFrame(testFrame);

		auto gameButton = createButton("Play");
		auto continueButton = createButton("Continue");
		auto optionsButton = createButton("Options");
		auto exitButton = createButton("Exit");

		gameButton->setEventCallback([=]() {this->game->setState(game->tdState); }, GUIEventType::MouseLeft_Up);
		gameButton->setEventCallback([=]() {this->game->setState(game->tdState); }, GUIEventType::MouseRight_Up);

		optionsButton->setEventCallback([=]() {this->game->setState(game->optionScreenState); }, GUIEventType::MouseLeft_Up);
		optionsButton->setEventCallback([=]() {this->game->setState(game->optionScreenState); }, GUIEventType::MouseRight_Up);
	
		exitButton->setEventCallback([=]() { Engine::Instance->stop(); }, GUIEventType::MouseLeft_Up);
		exitButton->setEventCallback([=]() {Engine::Instance->stop(); }, GUIEventType::MouseRight_Up);
		
		auto framePadding = GUI_Padding::Create(0.05f, 0.05f);

		testFrame->addChild(framePadding);

		auto verticalBox = GUI_VerticalBox::Create();
		verticalBox->addChild(gameButton);
		verticalBox->addChild(GUI_Padding::Create(0.025f, 0.025f));
		verticalBox->addChild(continueButton);
		verticalBox->addChild(GUI_Padding::Create(0.025f, 0.025f));
		verticalBox->addChild(optionsButton);
		verticalBox->addChild(GUI_Padding::Create(0.025f, 0.025f));
		verticalBox->addChild(exitButton);
		framePadding->addChild(verticalBox);


		//testFrame->setCullAABB(AABB<float>(0.0f, 0.0f, 2.0f));

		window.update(0.0f);

		OriginParticle FireParticle = OriginParticle();
		FireParticle.LifeSpanInitial = 0.8f;
		FireParticle.Position_X.SetupVariance(ParticleFunctions::Functions_Float.Function_LinearMechanic, { 0, 0, 0 }, { 11.45f, 0, 0.42f });
		FireParticle.Position_Y.SetupVariance(ParticleFunctions::Functions_Float.Function_LinearMechanic, { 0, 3.25f, 0.4f }, { 0, 1.5f, 0.02f });
		FireParticle.Position_Z.Setup(ParticleFloats.Function_Static, { 0.25f });
		FireParticle.Width.Setup(ParticleFunctions::Functions_Float.Function_BezierCurve, { 0.7f, 0.20f, 0.33f, 0.2f, 0.50f });
		FireParticle.Height.Setup(ParticleFunctions::Functions_Float.Function_BezierCurve, { 0.7f, 0.20f, 0.33f, 0.2f, 0.50f });

		FireParticle.ParticleColour.Setup(ParticleFunctions::Functions_TrueColour.Function_LinearInterp, { TrueColour(0, 0, 100, 100), TrueColour(50, 50, 100, 255), TrueColour(25, 25, 50, 0) });
		
		FireParticle.ParticleColour.SetupVariance(ParticleFunctions::Functions_TrueColour.Function_BezierCurve, FunctionBuilder::BuildBezierCurve<TrueColour>(TrueColour(),
		{ TrueColour(150,20, 0, 255), TrueColour(50, 40, 0, 200), TrueColour(100, 60, 0, 100), TrueColour(100, 60, 0, 100), TrueColour(50, 30, 200, 50), TrueColour(-255, -255, -255, -255) }), FunctionBuilder::BuildBezierCurve<TrueColour>(TrueColour(),
		{ TrueColour(0,40, 0, 0), TrueColour(20, 0, 0, 0), TrueColour(0, 40, 0, 0), TrueColour(0, 30, 0, 0), TrueColour(0, 10, 0, 0), TrueColour(-255, -255, -255, 0) }));

		ParticleEmitter FireEmitter = ParticleEmitter(FireParticle, 1055.0f);
		FireEmitter.SetTexture(TextureAsset::getTexture("particle"));


		OriginParticle SmokeParticle = OriginParticle();
		SmokeParticle.LifeSpanInitial = 4.0f;
		SmokeParticle.Position_X.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0, 0, 0 }, {11.85f, 0, 0.42f });
		SmokeParticle.Position_Y.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0.5f, 0.55f, 0.15f }, { 0.2f, 1.0f, 0 });
		SmokeParticle.Position_Z.Setup(ParticleFloats.Function_Static, { 0.25f });
		SmokeParticle.Width.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0.45f, 0, 0.1f }, { 0.05f, 0, 0.04f });
		SmokeParticle.Height.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0.45f, 0, 0.1f }, { 0.05f, 0, 0.04f });

		SmokeParticle.ParticleColour.Setup(ParticleColours.Function_LinearInterp, { TrueColour(), TrueColour(), TrueColour(40, 40, 40, 100), TrueColour(0, 0, 0, 100) });

		SmokeParticle.Radial.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0, 0, 0.1f }, { 0, 0, 0 });
		SmokeParticle.Tangential.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0, 0, 0 }, { 0, 0, 0.4f });


		ParticleEmitter SmokeEmitter = ParticleEmitter(SmokeParticle, 78.0f);
		SmokeEmitter.SetTexture(TextureAsset::getTexture("particle"));

		TestSystem.AddEmitter(FireEmitter);
		TestSystem.AddEmitter(SmokeEmitter);
		TestSystem.SetOffset(0.0f, -3.75f);
		TestSystem.update(2.0f);
		for (int i = 0; i < 50; ++i) {
			TestSystem.update(0.1f);
		}
	}

	void MenuState::update(const TimingType & deltaTime) {
		window.update(deltaTime);
		TestSystem.update(deltaTime);
		auto mouseLoc = game->getInputManager()->getMouseLocation();
		auto center = Camera::Cast<OrthographicCamera>(game->getCamera())->convertScreenToWorld(mouseLoc[0], mouseLoc[1]);
		//testFrame->setCullAABB(AABB<float>(center[0], center[1], 2.0f));
	}
	void MenuState::render(const TimingType & deltaTime, RenderingEngine * const engine) {
		window.render(deltaTime, engine);
		TestSystem.sendRenderingInformation(engine);
		//overlay->sendRenderInformation(engine);
		//cullingTest->sendRenderInformation(engine);
	}
}