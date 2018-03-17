#include "MenuState.h"

#include "RuntimeConstants.h"
#include "Conversion.h"

#include "Game.h"
#include "InputManager.h"
#include "GUI_Image.h"
#include "GUI_Label.h"
#include "GUI_HorizontalBox.h"

namespace Val {
	void MenuState::initialise() {
		auto image = SimpleRectangle();
		image.initialise(TextureAsset::getTexture(ResourceLocation("raven", ".png", RuntimeConstants::Instance->TexturePath)), 0, 0, 0.5f, 10.0f, 10.0f);
		auto test = GUI_Image::Create(image);

		auto image2 = SimpleRectangle();
		image2.initialise(TextureAsset::getTexture(ResourceLocation("raven", ".png", RuntimeConstants::Instance->TexturePath)), 0, 0, 0.5f, 10.0f, 10.0f);
		auto test3 = GUI_Image::Create(image2);

		auto textT = SimpleTextRectangle();
		textT.initialise(TextResource(FontAsset::getFont(ResourceLocation("ralewaymed", ".ttf", RuntimeConstants::Instance->FontPath), 128), "wau [?!]"), 1, 1, 0.5f, 6.0f, 6.0f);
		testLabel = GUI_Label::Create(textT);
		
		auto hBox = GUI_HorizontalBox::Create();

		auto imageh = SimpleRectangle();
		imageh.initialise(TextureAsset::getTexture(ResourceLocation("raven", ".png", RuntimeConstants::Instance->TexturePath)), 0, 0, 0.5f, 15.0f, 15.0f);
		auto test4 = GUI_Image::Create(imageh);

		hBox->addChild(test);
		hBox->addChild(test4);

		test->setJustification(LEFT, TOP);
		hBox->setJustification(LEFT, vCENTER);
		test3->setJustification(RIGHT, vCENTER);

		testBox.addChild(hBox);
		testBox.addChild(testLabel);
		testBox.addChild(test3);

		//test2->setHidden(true);
	}
	void MenuState::update(const TimingType & deltaTime) {

		if (game->getInputManager()->isAnyJustUp()) {
			testLabel->setHidden(!testLabel->isHidden());
		}
		testBox.update(deltaTime);
	}
	void MenuState::render(const TimingType & deltaTime, RenderingEngine * const engine) {
		testBox.render(deltaTime, engine);
	}
}