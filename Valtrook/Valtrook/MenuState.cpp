#include "MenuState.h"

#include "RuntimeConstants.h"
#include "Conversion.h"

#include "Game.h"
#include "InputManager.h"

#include "Logger.h"
#include "OrthographicCamera.h"
#include "AudioManager.h"
#include "Owner.h"


namespace Val {
	MenuState::MenuState(Game * const game) : GameState(game, "mainMenuState.chai"), window(game->getInputManager(), Camera::Cast<OrthographicCamera>(game->getCamera())) {
	}
	void MenuState::initialise() {
		GameState::initialise();

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
	}

	void MenuState::update(const TimingType & deltaTime) {
		GameState::update(deltaTime);

		window.update(deltaTime);
		auto mouseLoc = game->getInputManager()->getMouseLocation();
		auto center = Camera::Cast<OrthographicCamera>(game->getCamera())->convertScreenToWorld(mouseLoc[0], mouseLoc[1]);
		//testFrame->setCullAABB(AABB<float>(center[0], center[1], 2.0f));
	}
	void MenuState::render(const TimingType & deltaTime, RenderingEngine * const engine) {
		GameState::render(deltaTime, engine);

		window.render(deltaTime, engine);
		//overlay->sendRenderInformation(engine);
		//cullingTest->sendRenderInformation(engine);
	}
}