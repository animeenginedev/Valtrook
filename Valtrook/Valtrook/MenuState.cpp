#include "MenuState.h"

#include "RuntimeConstants.h"
#include "Conversion.h"

#include "Game.h"
#include "InputManager.h"

#include "Logger.h"
#include "OrthographicCamera.h"


namespace Val {
	void MenuState::initialise() {
		window.intialise(game->getInputManager(), Camera::Cast<OrthographicCamera>(game->getCamera()));
		testFrame = GUIFrame::Create();
		window.addFrame(testFrame);


		auto image = SimpleRectangle();
		image.initialise(TextureAsset::getTexture("raven"), 0, 0, 0.5f, 1.20f, 1.20f);

		auto test = GUI_Image::Create(image);
		auto test4 = GUI_Image::Create(image);

		image.setHalfSize(1.0f, 1.0f);
		auto test3 = GUI_Image::Create(image);
		auto test5 = GUI_Image::Create(image);

		auto textT = SimpleTextRectangle();
		textT.initialise(TextResource("wau", "ralewaymed"), 1, 1, 0.5f, 0.60f, 0.60f, Colour(0, 0, 0, 255));
		auto testLabel = GUI_Label::Create(textT);
		testLabel->setRecievesInputs(false);

		auto padding = GUI_Padding::Create(0.1f, 0.1f);
		padding->addChild(testLabel);
		padding->setRecievesInputs(false);

		auto testButton = GUI_Button::Create(0, 0, 0, 1, 1, TextureAsset::getTexture("gui/buttonDefault"), TextureAsset::getTexture("gui/buttonHover"), TextureAsset::getTexture("gui/buttonDown"));
		testButton->addChild(padding);

		auto table = GUI_Table::Create(3, 3);
				
		table->addChild(test, 0, 0);
		table->addChild(test4, 2, 2);
		table->addChild(testButton, 1, 1);
		table->addChild(test3, 2, 0);
		table->addChild(test5, 0, 2);

		test3->setJustification(LEFT, BOTTOM );
		test5->setJustification(RIGHT, TOP);

		testButton->setEventCallback([=]() { testLabel->setText("[?!]"); }, GUIEventType::MouseLeft_Up);
		test->setEventCallback([=]() { test->setHidden(!test->isHidden()); }, GUIEventType::MouseLeft_Up);
		test3->setEventCallback([=]() { test3->setJustification(RIGHT, TOP);
		test3->setEventCallback([=]() { test3->setHidden(true); }, GUIEventType::MouseLeft_Up); }, GUIEventType::MouseLeft_Up);
		test4->setEventCallback([=]() { test4->setHidden(!test4->isHidden()); }, GUIEventType::MouseLeft_Up);
		test5->setEventCallback([=]() { test5->setHidden(!test5->isHidden()); }, GUIEventType::MouseLeft_Up);

		testFrame->addChild(table);
	}
	void MenuState::update(const TimingType & deltaTime) {
		window.update(deltaTime);
	}
	void MenuState::render(const TimingType & deltaTime, RenderingEngine * const engine) {
		window.render(deltaTime, engine);
	}
}