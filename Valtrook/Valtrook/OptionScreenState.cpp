#include "OptionScreenState.h"

#include "Game.h"
#include "Particle.h"
#include "ParticleFunctions.h"
#include "MenuState.h"

namespace Val {

	OptionScreenState::OptionScreenState(Game * const game) : GameState(game), window(game->getInputManager(), Camera::Cast<OrthographicCamera>(game->getCamera())) {
	}
	void OptionScreenState::initialise() {
		OriginParticle SwirlParticle = OriginParticle();
		SwirlParticle.LifeSpanInitial = 4.0f;
		SwirlParticle.Position_X.SetupVariance(ParticleFunctions::Functions_Float.Function_LinearMechanic, { 0, 0, 0 }, { 0.25f, 0.3f, 0 });
		SwirlParticle.Position_Y.SetupVariance(ParticleFunctions::Functions_Float.Function_LinearMechanic, { 0, 0, 0 }, { 0.25f, 0.25f, 0 });
		SwirlParticle.Position_Z.Setup(ParticleFloats.Function_Static, { 0.25f });
		SwirlParticle.Width.SetupVariance(ParticleFunctions::Functions_Float.Function_LinearMechanic, { 0.1f, 0.04f, 0 }, { 0, 0.03f, 0 });
		SwirlParticle.Height.SetupVariance(ParticleFunctions::Functions_Float.Function_LinearMechanic, { 0.1f, 0.04f, 0 }, { 0, 0.03f, 0 });

		SwirlParticle.ParticleColour.SetupVariance(ParticleFunctions::Functions_TrueColour.Function_LinearInterp, { TrueColour(), TrueColour(),  TrueColour(50, 50, 50, 100), TrueColour(0, 0, 0, 255) }, 
		{ TrueColour(), TrueColour(),  TrueColour(200, 200, 200, 0), TrueColour(0, 0, 0, 0) });

		SwirlParticle.Radial.SetupVariance(ParticleFunctions::Functions_Float.Function_LinearMechanic, { 0, 0, 0.1f }, { 0, 4.15f, 0 });
		SwirlParticle.Tangential.SetupVariance(ParticleFunctions::Functions_Float.Function_LinearMechanic, { 0, 0, 0.1f }, { 0, 0.15f, 0.1f });

		ParticleEmitter SwirlEmitter = ParticleEmitter(SwirlParticle, 200.0f);
		SwirlEmitter.SetTexture(TextureAsset::getTexture("particle"));



		OriginParticle HyperSpaceParticle = OriginParticle();
		HyperSpaceParticle.LifeSpanInitial = 3.5f;
		HyperSpaceParticle.Position_X.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0, 0 }, { 40.0f, 0.0f });
		HyperSpaceParticle.Position_Y.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0, 0.81f }, { 40.0f, 0.0f });
		HyperSpaceParticle.Position_Z.Setup(ParticleFloats.Function_Static, { 0.25f });
		HyperSpaceParticle.Width.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0.25f, 0, 0.01f }, { 0.1f, 0, 0 });
		HyperSpaceParticle.Height.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0.25f, 0, 0.01f }, { 0.1f, 0, 0 });

		HyperSpaceParticle.ParticleColour.SetupVariance(ParticleFunctions::Functions_TrueColour.Function_BezierCurve, FunctionBuilder::BuildBezierCurve<TrueColour>(TrueColour(),
		{ TrueColour(150,10, 0, 255), TrueColour(50, 0, 200, 200), TrueColour(100, 10, 0, 100), TrueColour(100, 2000, 0, 100), TrueColour(0, -600, 0, 50), TrueColour(50, 1, 2000, 50), TrueColour(-255, -255, -255, -255) }), FunctionBuilder::BuildBezierCurve<TrueColour>(TrueColour(),
		{ TrueColour(0,40, 0, 0), TrueColour(20, 0, 150, 0), TrueColour(0, 140, 0, 0), TrueColour(0, 140, 0, 0), TrueColour(0, 110, 0, 0), TrueColour(1000, 100, 400, 0), TrueColour(-255, -255, -255, 0) }));
		
		HyperSpaceParticle.Radial.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0, 0, -3.81f }, { 0, 0, 0.4f });
		HyperSpaceParticle.Tangential.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0, 0, 0 }, { 0, 0, 0.2f });


		ParticleEmitter HyperSpaceEmitter = ParticleEmitter(HyperSpaceParticle, 500.0f);
		HyperSpaceEmitter.SetTexture(TextureAsset::getTexture("particle"));

		backgroundSystem.AddEmitter(SwirlEmitter);
		backgroundSystem.AddEmitter(HyperSpaceEmitter);

		auto testFrame = GUIFrameR::Create(1, 0.2f, Colour(55, 55, 55, 240), Colour(125, 150, 125, 240));
		window.addFrame(testFrame);

		auto verticalBox = GUI_VerticalBox::Create();
		testFrame->addChild(verticalBox);

		auto exitText = SimpleMultilineTextRectangle(TextResource("Exit", "ralewaymed", 128),
		{ 1, 1 }, 0.5f, 2.0f, 0.35f, Colour(0, 0, 0, 255));

		auto exitLabel = GUI_Label::Create(exitText);
		exitLabel->setRecievesInputs(false);

		auto exitPadding = GUI_Padding::Create(0.1f, 0.1f);
		exitPadding->addChild(exitLabel);
		exitPadding->setRecievesInputs(false);

		auto exitButton = GUI_Button::Create(0.15f, Colour(100, 100, 100, 230), Colour(80, 80, 80, 250), Colour(50, 50, 50, 255),
											 Colour(230, 230, 230, 230), Colour(200, 200, 200, 250), Colour(150, 150, 150, 255), FrameStyle::getDefault());

		exitButton->setEventCallback([=]() {this->game->setState(game->menuState); }, GUIEventType::MouseLeft_Up);
		exitButton->setEventCallback([=]() {this->game->setState(game->menuState); }, GUIEventType::MouseRight_Up);

		exitButton->addChild(exitPadding);
		verticalBox->addChild(exitButton);

		testFrame->setPosition({ 0, -3 });
		window.update(0.0f);
	}

	void OptionScreenState::update(const TimingType & deltaTime) {
		window.update(deltaTime);
		backgroundSystem.update(deltaTime);
	}

	void OptionScreenState::render(const TimingType & deltaTime, RenderingEngine * const engine) {
		window.render(deltaTime, engine);
		backgroundSystem.sendRenderingInformation(engine);
	}

}