#include "MenuState.h"

#include "RuntimeConstants.h"
#include "Conversion.h"

#include "Game.h"
#include "InputManager.h"

#include "Logger.h"
#include "OrthographicCamera.h"
#include "AudioManager.h"
#include "Owner.h"

#include "Particle.h"
#include "ParticleFunctions.h"

namespace Val {
	MenuState::MenuState(Game * const game) : GameState(game), window(game->getInputManager(), Camera::Cast<OrthographicCamera>(game->getCamera())) {
	}
	void MenuState::initialise() {
		testFrame = GUIFrameR::Create(1, 0.2f, Colour(55, 55, 55, 255), Colour(125, 150, 125, 255));
		window.addFrame(testFrame);


		auto image = SimpleRectangle(TextureAsset::getTexture("raven"), 0, 0, 0.5f, 1.20f, 3.20f);

		overlay = new SimpleRectangle(image);
		overlay->setHalfSize(3.0f, 3.0f);
		overlay->setColour(Colour(255, 255, 255, 100));

		cullingTest = new SimpleRectangle(image);
		cullingTest->setHalfSize(3.0f, 3.0f);
		cullingTest->setCullSurface(AABB<float>(-1.0f, 1.0f, 1.5f, 3.5f));

		auto test = GUI_Image::Create(image);
		auto test4 = GUI_Image::Create(image);

		image.setHalfSize(1.0f, 1.0f);
		auto test3 = GUI_Image::Create(image);
		auto test5 = GUI_Image::Create(image);

		auto textT = SimpleMultilineTextRectangle(TextResource("Multiline text rendering is implemented im so fucking good, i've been worried about this autisim for ages holy shit. AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", "ralewaymed", 128),
		{ 1, 1 }, 0.5f, 2.0f, 0.35f, Colour(0, 0, 0, 255));
		textT.setJustification(LEFT);
		//textT.setScaleTextToHeight(true);
		auto testLabel = GUI_Label::Create(textT);
		testLabel->setRecievesInputs(false);

		auto padding = GUI_Padding::Create(0.1f, 0.1f);
		padding->addChild(testLabel);
		padding->setRecievesInputs(false);

		auto testButton = GUI_Button::Create(0.15f, Colour(100, 100, 100, 255), Colour(80, 80, 80, 255), Colour(50, 50, 50, 255), 
											 Colour(230, 230, 230, 255), Colour(200, 200, 200, 255), Colour(150, 150, 150, 255), FrameStyle::getDefault());
		testButton->addChild(padding);

		auto table = GUI_Table::Create(4,4);
				
		table->addChild(test, 0, 0);
		table->addChild(test4, 2, 3);
		table->addChild(testButton, 1, 1);
		table->addChild(test3, 2, 0);
		table->addChild(test5, 0, 2);

		test3->setJustification(LEFT, BOTTOM );
		test5->setJustification(RIGHT, TOP);
				

		testButton->setEventCallback([=]() { testLabel->setText("never ever"); }, GUIEventType::MouseLeft_Up);
		test->setEventCallback([=]() { test->setHidden(!test->isHidden()); }, GUIEventType::MouseLeft_Up);
		test3->setEventCallback([=]() { test3->setJustification(RIGHT, TOP);
		test3->setEventCallback([=]() { test3->setHidden(true); }, GUIEventType::MouseLeft_Up); }, GUIEventType::MouseLeft_Up);
		test4->setEventCallback([=]() { 
			test4->setHidden(!test4->isHidden());
			auto audioP = game->getAudioManager()->getAudioPlayer(ResourceLocation("knockknockknock", ".ogg", RuntimeConstants::Instance->SoundPath), true);
			audioP->play([=](AudioDelegate* d) { audioP->playFadeIn(100); }, 0);
		
		}, GUIEventType::MouseLeft_Up);
		test5->setEventCallback([=]() { test5->setHidden(!test5->isHidden()); }, GUIEventType::MouseLeft_Up);

		testScroll = GUI_VerticalScroll::Create({ 4, 3 }, 0.1f, TextureAsset::getTexture("gui/scrollbarCircleUp"), TextureAsset::getTexture("gui/scrollbarCircleDown"), TextureAsset::getTexture("gui/scrollbarCircleHover"));
		auto framePadding = GUI_Padding::Create(0.05f, 0.05f);

		testFrame->addChild(framePadding);
		testFrame->setPosition({ 1, -0.5f });

		framePadding->addChild(testScroll);

		testScroll->addChild(table);

		//testFrame->setCullAABB(AABB<float>(0.0f, 0.0f, 2.0f));

		window.update(0.0f);

		OriginParticle FireParticle = OriginParticle();
		FireParticle.LifeSpanInitial = 3.0f;
		FireParticle.Position_X.SetupVariance(ParticleFunctions::Functions_Float.Function_LinearMechanic, { 0, 0, 0 }, { 0.45f, 0, 0.42f });
		FireParticle.Position_Y.SetupVariance(ParticleFunctions::Functions_Float.Function_LinearMechanic, { 0, 0.25f, 0.4f }, { 0, 0.1f, 0.02f });
		FireParticle.Position_Z.Setup(ParticleFloats.Function_Static, { 0.25f });
		FireParticle.Width.Setup(ParticleFunctions::Functions_Float.Function_BezierCurve, { 0.25f, 0.20f, 0.33f, 0.2f, 0.50f });
		FireParticle.Height.Setup(ParticleFunctions::Functions_Float.Function_BezierCurve, { 0.25f, 0.20f, 0.33f, 0.2f, 0.50f });

		FireParticle.ParticleColour.Setup(ParticleFunctions::Functions_TrueColour.Function_LinearInterp, { TrueColour(0, 0, 100, 100), TrueColour(50, 50, 100, 255), TrueColour(25, 25, 50, 0) });

		FireParticle.Radial.Setup(ParticleFunctions::Functions_Float.Function_LinearMechanic, { 0, 0, -0.1f });
		FireParticle.Tangential.SetupVariance(ParticleFunctions::Functions_Float.Function_LinearMechanic, { 0, 0, 0 }, { 0, 0, 0.2f });

		FireParticle.ParticleColour.SetupVariance(ParticleFunctions::Functions_TrueColour.Function_BezierCurve, FunctionBuilder::BuildBezierCurve<TrueColour>(TrueColour(),
		{ TrueColour(150,10, 0, 255), TrueColour(50, 0, 0, 200), TrueColour(100, 10, 0, 100), TrueColour(100, 20, 0, 100), TrueColour(50, 10, 0, 50), TrueColour(50, 1, 0, 50), TrueColour(-255, -255, -255, -255) }), FunctionBuilder::BuildBezierCurve<TrueColour>(TrueColour(),
		{ TrueColour(0,40, 0, 0), TrueColour(20, 0, 0, 0), TrueColour(0, 40, 0, 0), TrueColour(0, 40, 0, 0), TrueColour(0, 10, 0, 0), TrueColour(0, 0, 0, 0), TrueColour(-255, -255, -255, 0) }));

		ParticleEmitter FireEmitter = ParticleEmitter(FireParticle, 200.0f);
		FireEmitter.SetTexture(TextureAsset::getTexture("particle"));


		OriginParticle SmokeParticle = OriginParticle();
		SmokeParticle.LifeSpanInitial = 4.0f;
		SmokeParticle.Position_X.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0, 0, 0 }, { 0.85f, 0, 0.42f });
		SmokeParticle.Position_Y.SetupVariance(ParticleFloats.Function_LinearMechanic, { 1.0f, 0.55f, 0.15f }, { 0.2f, 1.0f, 0 });
		SmokeParticle.Position_Z.Setup(ParticleFloats.Function_Static, { 0.25f });
		SmokeParticle.Width.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0.45f, 0, 0.1f }, { 0.05f, 0, 0.04f });
		SmokeParticle.Height.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0.45f, 0, 0.1f }, { 0.05f, 0, 0.04f });

		SmokeParticle.ParticleColour.Setup(ParticleColours.Function_LinearInterp, { TrueColour(), TrueColour(), TrueColour(40, 40, 40, 100), TrueColour(0, 0, 0, 100) });

		SmokeParticle.Radial.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0, 0, 0.1f }, { 0, 0, 0 });
		SmokeParticle.Tangential.SetupVariance(ParticleFloats.Function_LinearMechanic, { 0, 0, 0 }, { 0, 0, 0.4f });


		ParticleEmitter SmokeEmitter = ParticleEmitter(SmokeParticle, 25.0f);
		SmokeEmitter.SetTexture(TextureAsset::getTexture("particle"));

		TestSystem.AddEmitter(FireEmitter);
		TestSystem.AddEmitter(SmokeEmitter);
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